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

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#pragma comment(lib, "ws2_32.lib")

#define  MAX_RECV_BUF_SIZE  4096

class TcpServer 
{
public:
	/* 启动服务器 */
	static void Start(int port)
	{
		event_base* base;
		evconnlistener* listener;
		event* signal_event;

		sockaddr_in sin;
		#ifdef _WIN32
			WSADATA wsa_data;
			WSAStartup(0x0201, &wsa_data);
		#endif

		base = event_base_new();
		if (!base) {
			fprintf(stderr, "Could not initialize libevent!\n");
			return;
		}

		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);

		listener = evconnlistener_new_bind(base, Listen, (void *)base,
			LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
			(struct sockaddr*)&sin,
			sizeof(sin));

		if (!listener) {
			fprintf(stderr, "Could not create a listener!\n");
			return;
		}

		signal_event = evsignal_new(base, SIGINT, Signal_cb, (void *)base);

		if (!signal_event || event_add(signal_event, NULL)<0) {
			fprintf(stderr, "Could not create/add a signal event!\n");
			return;
		}

		event_base_dispatch(base); // 进入事件循环...

		evconnlistener_free(listener);
		event_free(signal_event);
		event_base_free(base);
	}

	/* 监听事件 */
	static void Listen(evconnlistener* listener, evutil_socket_t fd, sockaddr* sa, int socklen, void* user_data)
	{
		event_base* base = (event_base*)user_data;
		bufferevent* bufev;

		bufev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
		if (!bufev) {
			fprintf(stderr, "Error constructing bufferevent!");
			event_base_loopbreak(base);
			return;
		}
		bufferevent_setcb(bufev, Read_cb, Write_cb, Connect_cb, NULL);
		bufferevent_enable(bufev, EV_WRITE| EV_READ);

		// 打印连接信息
		sockaddr_in* addrinfo = (sockaddr_in*)sa;
		printf("fd:%d sockaddr:%s port:%d \n", fd, inet_ntoa(addrinfo->sin_addr), ntohs(addrinfo->sin_port));

		string str("Connetion server success!!");
		bufferevent_write(bufev, str.c_str(), str.size());
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
		} 
		while (0 < (data_len -= len)); // 一次性读完缓冲区所有数据

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
		/* None of the other events can happen here, since we haven't enabled
		* timeouts */
		bufferevent_free(bufev);
	}

	// 信号回调
	static void Signal_cb(evutil_socket_t sig, short events, void* user_data)
	{
		event_base* base = (event_base*)user_data;
		timeval delay = { 2, 0 };

		printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

		event_base_loopexit(base, &delay);
	}

};

int main(int argc, char **argv)
{
	int port = 9995; // 连接端口

	TcpServer::Start(port);

	getchar();
	return 0;
}