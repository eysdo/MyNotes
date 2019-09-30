#pragma once

#ifdef _MSC_VER // Windows
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  // ���#include <WinSock2.h> �������ض��������
#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_ntoa()��ʱ����

#include <Windows.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
//#define socklen_t int
#define sleep  Sleep

#elif __GNUC__ // Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#define closesocket close
#endif

#include <iostream>
#include <stdexcept>
#include <set>
using namespace std;

#include <event.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
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

class TcpServer
{
private:
	SOCKET _sock; // socket�ļ�������
	static set<SOCKET> _cfdsPool;
	static queue<PkgHeader*> _packet_queue; // ���ݰ�����
	static mutex _mux; // ������


public:
	TcpServer()
	{
		_sock = INVALID_SOCKET;

		// ����Win Socket 2.x����
#ifdef _MSC_VER // Windows
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		WSAStartup(ver, &dat);
#endif
	}
	~TcpServer()
	{
		Close();
		// ����Win Socket 2.x����ռ����Դ
#ifdef _MSC_VER // Windows
		WSACleanup();
#endif
	}

	/* ��ʼ�� Socket */
	int Init()
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

		// �����ΰ�ͬһ����ַ��Ҫ����socket��bind֮��
		evutil_make_listen_socket_reuseable(_sock);
	}

	/* �󶨶�IP�Ϳں� */
	void Bind(const unsigned short port, const char* ip = NULL)
	{
		sockaddr_in _sin;
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port); // host to net unsigned short
		_sin.sin_addr.s_addr = NULL == ip ? INADDR_ANY : inet_addr(ip);// INADDR_ANY ͬ inet_addr("0.0.0.0");
		if (SOCKET_ERROR == ::bind(_sock, (sockaddr*)&_sin, (socklen_t)sizeof(_sin)))
		{
			throw runtime_error("ERROR:bind(),�󶨶˿�ʧ��!!\n");
		}
	}

	/* �����˿ں� */
	void Listen(int backlog)
	{
		if (SOCKET_ERROR == listen(_sock, backlog))
		{
			throw runtime_error("ERROR:listen(),��������˿�ʧ��!!\n");
		}
		// ��ƽ̨ͳһ�ӿڣ����׽�������Ϊ������״̬
		evutil_make_socket_nonblocking(_sock);
	}

	/* �󶨶�IP�Ϳں� ͬʱ����, һ����λ */
	void Listen(const unsigned short port, const char* ip = NULL, int backlog = 10)
	{
		if (INVALID_SOCKET == _sock)
		{
			Init(); // ���δ��ʼ������,�Զ���ʼ��Socket
		}
		Bind(port, ip);
		Listen(backlog);
	}

	/* ��ȡ�ͻ������� */
	static void Accept_cb(int fd, short events, void* user_arg)
	{
		sockaddr_in clienAddr;
		socklen_t addrlen = sizeof(clienAddr);
		SOCKET cSock = INVALID_SOCKET;

		cSock = accept(fd, (sockaddr*)&clienAddr, &addrlen);
		if (INVALID_SOCKET == cSock)
		{
			cout << "ERROR:accept(),���յ���Ч�Ŀͻ���SOCKET!!" << endl;
			return;
		}

		// ���÷�����
		evutil_make_socket_nonblocking(cSock);

		event_base* base = (event_base*)user_arg;
		//������Ϊ�˶�̬����һ��event�ṹ��
		event *ev = event_new(NULL, -1, 0, NULL, NULL);
		event_assign(ev, base, cSock, EV_READ | EV_PERSIST,
			Read_cb, (void*)ev);

		event_add(ev, NULL);

		_cfdsPool.insert(cSock);
		cout << "�����ӿͻ���IP:" << inet_ntoa(clienAddr.sin_addr)
			<< " �˿�:" << clienAddr.sin_port << endl;
	}

	/* ����ָ���û����� */
	static int SendData(SOCKET cSock = INVALID_SOCKET, PkgHeader *header = NULL)
	{
		if (cSock != INVALID_SOCKET && header)
		{
			return send(cSock, (const char*)header, header->pkg_size, 0);
		}
		return SOCKET_ERROR;
	}

	/* ���͹㲥���� */
	void SendDataAll(PkgHeader *header)
	{
		if (header)
		{
			for (set<SOCKET>::iterator it = _cfdsPool.begin(); it != _cfdsPool.end(); it++)
			{
				send(*it, (const char*)header, header->pkg_size, 0);
			}
		}
	}

	/* �������� ����ճ������ְ� */
	static void Read_cb(int c_fd, short events, void *user_arg)
	{
		// ���ջ�����
		static char recvBuf[RECV_BUFF_SIZE] = { 0 };
		// �ڶ������� ��Ϣ������
		static char secondBuf[RECV_BUFF_SIZE * 3500] = { 0 };
		// ��Ϣ������������β��λ��
		static int lastPos = 0;

		// ���շ�����������
		int recvlen = recv(c_fd, recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "�ͻ���<Socket:" << c_fd << ">���˳�, �������!" << endl;
			_cfdsPool.erase(c_fd);
			event_free((event*)user_arg);
			closesocket(c_fd);
			c_fd = INVALID_SOCKET;
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
				DataPacket* pkt = (DataPacket*)secondBuf;
				//cout << "recvlen = " << recvlen << endl;
				//cout << "data" << msg->data << endl;

				string pkt_src_data;
				pkt_src_data.assign(pkt->data, pkt->data_size);
				string pkt_data(move(des_decrypt(gzip_uncompress(pkt_src_data), "12345678")));

				// �������ݰ�, �������ݰ�����
				DataPacket* cpy_pkt = new DataPacket;
				cpy_pkt->data_size = pkt_data.size();
				memcpy(cpy_pkt->data, pkt_data.c_str(), cpy_pkt->data_size);
				
				_mux.lock();
				_packet_queue.push(cpy_pkt);
				_mux.unlock();


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
	void Start()
	{
		event_base* base = event_base_new();
		event* ev_listen = event_new(base, _sock, EV_READ | EV_PERSIST,
			Accept_cb, base);
		event_add(ev_listen, NULL);

		event_base_dispatch(base); // ����, �����¼�ѭ��..

		event_free(ev_listen);
		event_base_free(base);
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

	/* �۲쵱ǰ�ͻ������������� */
	void Wacth()
	{
		while (true)
		{
			cout << "��ǰ��������:" << _cfdsPool.size() << endl;
			sleep(1000);
		}
	}

	/* ���ݰ������߳� */
	void packetWork()
	{
		while (true)
		{
			_mux.lock();
			if (!_packet_queue.size())
			{
				_mux.unlock();
				sleep(5);
				continue;
			}

			DataPacket* pkt = (DataPacket*)_packet_queue.front();
			cout << "data:" << pkt->data << endl;
			cout << "data_size:" << pkt->data_size << endl;
			delete pkt;
			_packet_queue.pop();
			_mux.unlock();
			sleep(5);
		}
	}
};

// �ͻ������ӳ�����
set<SOCKET> TcpServer::_cfdsPool = {};
queue<PkgHeader*> TcpServer::_packet_queue; // ���ݰ�����
mutex TcpServer::_mux; // ������