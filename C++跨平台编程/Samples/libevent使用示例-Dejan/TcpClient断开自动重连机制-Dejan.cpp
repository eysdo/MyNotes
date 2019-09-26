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
		// ����Win Socket 2.x����ռ����Դ
		#ifdef _MSC_VER // Windows
				WSACleanup();
		#endif
	}

	void Connect(const char* ip, int port)
	{
		// �����Զ���������
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

			event_base_dispatch(base); // �����¼�ѭ��...

			event_base_free(base);

			// CPU��Ϣһ��
			Sleep(2);
		}
		
	}

	/* ���¼��ص� */
	static void Read_cb(bufferevent* bufev, void* user_data)
	{
		// ��ȡ���Ͷ�������Ϣ fd��IP��ַ���˿�
		int fd = bufferevent_getfd(bufev);
		sockaddr_in sa;
		int sa_len = sizeof(sa);
		getpeername(fd, (sockaddr*)&sa, &sa_len);
		printf("fd:%d sockaddr:%s port:%d \n", fd, inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));

		// ��ӡ���������ܳ���
		evbuffer* input = bufferevent_get_input(bufev);
		long long data_len = evbuffer_get_length(input);
		printf("evbuffer_get_length: %d \n", data_len);

		// ���յ�����������
		string data;

		int len = 0; // һ�ν������ݵĳ���, һ��send�ж��recv..
		do {
			char recv_buf[MAX_RECV_BUF_SIZE] = { 0 };
			len = bufferevent_read(bufev, recv_buf, MAX_RECV_BUF_SIZE);
			data.append(recv_buf, len);
		} while (0 < (data_len -= len)); // һ���Զ��껺������������

										 // ��ӡ������Ϣ
		printf("%s \n", data.c_str());

		// ת���ظ��ͻ���
		bufferevent_write(bufev, data.c_str(), data.size());
	}

	/* д�¼��ص� */
	static void Write_cb(bufferevent* bufev, void* user_data)
	{
		// ...
	}

	/* �������¼��ص� */
	static void Connect_cb(struct bufferevent *bufev, short events, void *user_data)
	{
		if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) // �����Զ���������
		{
			// BEV_EVENT_EOF   => printf("Connection closed.\n");
			// BEV_EVENT_ERROR => printf("Got an error on the connection: %s\n", strerror(errno));
			if (user_data)
			{
				printf("����Ͽ���������... \n");
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