#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
using namespace std;

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef _WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include<event.h>
#include<event2/bufferevent.h>
#include<event2/buffer.h>
#include<event2/util.h>

#pragma comment(lib, "ws2_32.lib")

#define  MAX_RECV_BUF_SIZE  4096

class TcpClient
{
private:
	static sockaddr_in mServerAddr;

public:
	TcpClient() 
	{
		#ifdef _WIN32
				WSADATA wsa_data;
				WSAStartup(0x0201, &wsa_data);
		#endif
	}
	~TcpClient()
	{
		// 回收Win Socket 2.x环境占用资源
		#ifdef _MSC_VER // Windows
				WSACleanup();
		#endif
	}

	void Connect(const char* ip, int port)
	{
		// 断网自动重连机制
		while (true)
		{
			event_base* base = event_base_new();
			bufferevent* bufev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

			mServerAddr.sin_family = AF_INET;
			mServerAddr.sin_port = htons(port);
			mServerAddr.sin_addr.s_addr = inet_addr(ip);

			bufferevent_setcb(bufev, Read_cb, Write_cb, Connect_cb, base);
			bufferevent_enable(bufev, EV_READ | EV_WRITE | EV_PERSIST);

			bufferevent_socket_connect(bufev, (sockaddr*)&mServerAddr, sizeof(mServerAddr));

			event_base_dispatch(base); // 进入事件循环...

			event_base_free(base);

			// CPU休息一会
			Sleep(2);
		}
		
	}

	/* 读事件回调 */
	static void Read_cb(bufferevent* bufev, void* user_data)
	{
		// 获取发送端连接信息 fd、IP地址、端口
		int fd = bufferevent_getfd(bufev);
		sockaddr_in sa;
		int sa_len = sizeof(sa);
		getpeername(fd, (sockaddr*)&sa, &sa_len);
		printf("fd:%d sockaddr:%s port:%d \n", fd, inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));

		// 打印接收数据总长度
		evbuffer* input = bufferevent_get_input(bufev);
		long long data_len = evbuffer_get_length(input);
		printf("evbuffer_get_length: %d \n", data_len);

		// 接收到的完整数据
		string data;

		int len = 0; // 一次接收数据的长度, 一次send有多次recv..
		do {
			char recv_buf[MAX_RECV_BUF_SIZE] = { 0 };
			len = bufferevent_read(bufev, recv_buf, MAX_RECV_BUF_SIZE);
			data.append(recv_buf, len);
		} while (0 < (data_len -= len)); // 一次性读完缓冲区所有数据

										 // 打印发送信息
		printf("%s \n", data.c_str());

		// 转发回给客户端
		bufferevent_write(bufev, data.c_str(), data.size());
	}

	/* 写事件回调 */
	static void Write_cb(bufferevent* bufev, void* user_data)
	{
		// ...
	}

	/* 新连接事件回调 */
	static void Connect_cb(struct bufferevent *bufev, short events, void *user_data)
	{
		if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) // 断网自动重连机制
		{
			// BEV_EVENT_EOF   => printf("Connection closed.\n");
			// BEV_EVENT_ERROR => printf("Got an error on the connection: %s\n", strerror(errno));
			if (user_data)
			{
				printf("网络断开正在重连... \n");
				event_base_loopbreak((event_base*)user_data);
			}
		}
	}
};

sockaddr_in TcpClient::mServerAddr = {};

int main(int argc, char **argv)
{
	const char* ip = "127.0.0.1";
	int port = 9995;

	unique_ptr<TcpClient> client(new TcpClient);
	client->Connect(ip, port);

	getchar();
	return 0;
}