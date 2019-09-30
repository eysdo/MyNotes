#pragma once

#ifdef _MSC_VER // Windows
#define WIN32_LEAN_AND_MEAN  // 解决#include <WinSock2.h> 里面有重定义的问题
#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_ntoa()、inet_addr()过时问题

#include <Windows.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#elif __GNUC__ // Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#define closesocket close
#endif

#include <iostream>
#include <stdexcept>
using namespace std;

#include <event.h>
#include <event2/util.h>
#include <thread>
#include "Des.h"
#include "Gzip.h"

struct PkgHeader
{
	int pkg_size;  // 数据包总长度(大小)
};
struct DataPacket : public PkgHeader
{
	DataPacket()
	{
		pkg_size = sizeof(DataPacket);
	}
	char data[4096] = { 0 };
	int  data_size = 0;
};

#define RECV_BUFF_SIZE 65535

class TcpClient
{
private:
	static SOCKET _sock; // socket文件描述符

public:
	TcpClient()
	{
		_sock = INVALID_SOCKET;
		// 启动Win Socket 2.x环境
#ifdef _MSC_VER // Windows
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		WSAStartup(ver, &dat);
#endif
	}
	~TcpClient()
	{
		Close();
		// 回收Win Socket 2.x环境占用资源
#ifdef _MSC_VER // Windows
		WSACleanup();
#endif
	}

	/* 初始化 socket */
	void Init()
	{
		if (INVALID_SOCKET != _sock)
		{
			Close(); // 防止重新连接服务器失败
			printf("[sock:%d]Disconnect old connection successfully!\n", _sock);
		}

		_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == _sock)
		{
			throw std::runtime_error("ERROR:socket(),Create socket failed!!\n");
		}
	}

	/* 连接服务器 */
	void Connect(const char* ip, unsigned short port)
	{
		// 断开自动重连机制
		while (true)
		{
			Init(); // 初始化 socket

			sockaddr_in _sin;
			_sin.sin_family = AF_INET;
			_sin.sin_port = htons(port);
			_sin.sin_addr.s_addr = inet_addr(ip);
			while (SOCKET_ERROR == connect(_sock, (sockaddr*)&_sin, sizeof(_sin)))
			{
				//printf("ERROR:connect(),Connect server failed!!\n");
				Sleep(1000);
				printf("连接已断开了,正在重新连接... \n");
			}

			// 设置非阻塞
			evutil_make_socket_nonblocking(_sock);

			event_base* base = event_base_new();
			event *ev_sockfd = event_new(base, _sock,
				EV_READ | EV_PERSIST,
				Read_cb, base);
			event_add(ev_sockfd, NULL);

			event_base_dispatch(base); // 阻塞, 进入事件循环..

			cout << "事件循环退出..." << endl;
			event_free(ev_sockfd);
			event_base_free(base);
		}
	}

	/* 接收信息回调 & 数据拆分包处理 */
	static void Read_cb(int fd, short events, void *user_arg)
	{
		// 接收缓冲区
		static char recvBuf[RECV_BUFF_SIZE] = { 0 };
		// 第二缓冲区 消息缓冲区
		static char secondBuf[RECV_BUFF_SIZE * 10] = { 0 };
		// 消息缓冲区的数据尾部位置
		static int lastPos = 0;

		// 5 接收服务器端数据
		int recvlen = recv(fd, recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "与服务器断开连接, 任务结束!" << endl;
			// 退出event事件循环
			event_base_loopbreak((event_base*)user_arg); // 断开自动重连机制
			return;
		}

		// 将收取到的数据拷贝到消息缓冲区
		memcpy(secondBuf + lastPos, recvBuf, recvlen);
		lastPos += recvlen; // 消息缓存区的数据尾部位置后移
		if (lastPos >= sizeof(PkgHeader))
		{
			PkgHeader* header = (PkgHeader*)secondBuf;
			if (lastPos >= header->pkg_size)
			{
				DataPacket* msg = (DataPacket*)secondBuf;
				cout << "recvlen = " << recvlen << endl;
				cout << msg->data << endl;

				// 剩余未处理消息缓冲区数据的长度
				int nSize = lastPos - header->pkg_size;
				// 将消息缓存区剩余未处理数据前移
				memcpy(secondBuf, secondBuf + header->pkg_size, nSize);
				// 消息缓冲区的数据尾部位置前移
				lastPos = nSize;
			}
		}
	}

	/* 发送数据 */
	void SendData(const string& data)
	{
		while (_sock == INVALID_SOCKET)
		{
			Sleep(5);
		}
		string s_data(move(gzip_compress(des_encrypt(data, "12345678"))));
		DataPacket pkt;
		pkt.data_size = s_data.size();
		memcpy(pkt.data, s_data.c_str(), pkt.data_size);
		
	start_send:
		int send_data_len = ::send(_sock, (const char*)&pkt, pkt.pkg_size, 0);
		if (send_data_len < 0) goto start_send;
	}

	/* Socket清理&关闭 */
	void Close()
	{
		if (INVALID_SOCKET != _sock)
		{
			// 关闭套接字 closesocket
			closesocket(_sock);

			_sock = INVALID_SOCKET;
		}
	}

};

// socket文件描述符
SOCKET TcpClient::_sock = INVALID_SOCKET;
