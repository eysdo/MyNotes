#pragma once

#ifdef _MSC_VER // Windows
#define WIN32_LEAN_AND_MEAN  // ���#include <WinSock2.h> �������ض��������
#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_ntoa()��inet_addr()��ʱ����

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
	int pkg_size;  // ���ݰ��ܳ���(��С)
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
	static SOCKET _sock; // socket�ļ�������

public:
	TcpClient()
	{
		_sock = INVALID_SOCKET;
		// ����Win Socket 2.x����
#ifdef _MSC_VER // Windows
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		WSAStartup(ver, &dat);
#endif
	}
	~TcpClient()
	{
		Close();
		// ����Win Socket 2.x����ռ����Դ
#ifdef _MSC_VER // Windows
		WSACleanup();
#endif
	}

	/* ��ʼ�� socket */
	void Init()
	{
		if (INVALID_SOCKET != _sock)
		{
			Close(); // ��ֹ�������ӷ�����ʧ��
			printf("[sock:%d]Disconnect old connection successfully!\n", _sock);
		}

		_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == _sock)
		{
			throw std::runtime_error("ERROR:socket(),Create socket failed!!\n");
		}
	}

	/* ���ӷ����� */
	void Connect(const char* ip, unsigned short port)
	{
		// �Ͽ��Զ���������
		while (true)
		{
			Init(); // ��ʼ�� socket

			sockaddr_in _sin;
			_sin.sin_family = AF_INET;
			_sin.sin_port = htons(port);
			_sin.sin_addr.s_addr = inet_addr(ip);
			while (SOCKET_ERROR == connect(_sock, (sockaddr*)&_sin, sizeof(_sin)))
			{
				//printf("ERROR:connect(),Connect server failed!!\n");
				Sleep(1000);
				printf("�����ѶϿ���,������������... \n");
			}

			// ���÷�����
			evutil_make_socket_nonblocking(_sock);

			event_base* base = event_base_new();
			event *ev_sockfd = event_new(base, _sock,
				EV_READ | EV_PERSIST,
				Read_cb, base);
			event_add(ev_sockfd, NULL);

			event_base_dispatch(base); // ����, �����¼�ѭ��..

			cout << "�¼�ѭ���˳�..." << endl;
			event_free(ev_sockfd);
			event_base_free(base);
		}
	}

	/* ������Ϣ�ص� & ���ݲ�ְ����� */
	static void Read_cb(int fd, short events, void *user_arg)
	{
		// ���ջ�����
		static char recvBuf[RECV_BUFF_SIZE] = { 0 };
		// �ڶ������� ��Ϣ������
		static char secondBuf[RECV_BUFF_SIZE * 10] = { 0 };
		// ��Ϣ������������β��λ��
		static int lastPos = 0;

		// 5 ���շ�����������
		int recvlen = recv(fd, recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "��������Ͽ�����, �������!" << endl;
			// �˳�event�¼�ѭ��
			event_base_loopbreak((event_base*)user_arg); // �Ͽ��Զ���������
			return;
		}

		// ����ȡ�������ݿ�������Ϣ������
		memcpy(secondBuf + lastPos, recvBuf, recvlen);
		lastPos += recvlen; // ��Ϣ������������β��λ�ú���
		if (lastPos >= sizeof(PkgHeader))
		{
			PkgHeader* header = (PkgHeader*)secondBuf;
			if (lastPos >= header->pkg_size)
			{
				DataPacket* msg = (DataPacket*)secondBuf;
				cout << "recvlen = " << recvlen << endl;
				cout << msg->data << endl;

				// ʣ��δ������Ϣ���������ݵĳ���
				int nSize = lastPos - header->pkg_size;
				// ����Ϣ������ʣ��δ��������ǰ��
				memcpy(secondBuf, secondBuf + header->pkg_size, nSize);
				// ��Ϣ������������β��λ��ǰ��
				lastPos = nSize;
			}
		}
	}

	/* �������� */
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

	/* Socket����&�ر� */
	void Close()
	{
		if (INVALID_SOCKET != _sock)
		{
			// �ر��׽��� closesocket
			closesocket(_sock);

			_sock = INVALID_SOCKET;
		}
	}

};

// socket�ļ�������
SOCKET TcpClient::_sock = INVALID_SOCKET;
