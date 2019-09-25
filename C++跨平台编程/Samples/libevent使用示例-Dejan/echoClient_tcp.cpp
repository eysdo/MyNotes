#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
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
public:
	static void Start(const char* ip, int port) 
	{
#ifdef _WIN32
		WSADATA wsa_data;
		WSAStartup(0x0201, &wsa_data);
#endif

		event_base* base = event_base_new();
		bufferevent* bufev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

		sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = inet_addr(ip);

		bufferevent_setcb(bufev, Read_cb, Write_cb, Connect_cb, NULL);
		bufferevent_enable(bufev, EV_READ | EV_WRITE | EV_PERSIST);

		while (bufferevent_socket_connect(bufev, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		{
			printf("Client bufferevent_socket_connect Error!\n");
			bufferevent_free(bufev);
			Sleep(3000);
			// 3秒后重连...
		}

		event_base_dispatch(base); // 进入事件循环...

		event_base_free(base);
	}

	/* 读事件回调 */
	static void Read_cb(bufferevent* bufev, void* user_data)
	{
		cout << "我被执行" << endl;
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
		if (events & BEV_EVENT_EOF) {
			printf("Connection closed.\n");
		}
		else if (events & BEV_EVENT_ERROR) {
			printf("Got an error on the connection: %s\n",
				strerror(errno));/*XXX win32*/
		}
		else if (events & BEV_EVENT_CONNECTED)
		{
			printf("the client has connected to server\n");
		}
		/* None of the other events can happen here, since we haven't enabled
		* timeouts */
		//bufferevent_free(bufev);
	}
};

int main(int argc, char **argv)
{
	const char* ip = "127.0.0.1";
	int port = 9995;

	TcpClient::Start(ip, port);

	getchar();
	return 0;
}