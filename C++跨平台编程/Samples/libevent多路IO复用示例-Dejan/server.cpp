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

struct DataHeader
{
	short dataLength;  // ���ݳ���
};
struct Msg :public DataHeader
{
	Msg()
	{
		dataLength = sizeof(Msg);
	}
	char data[1024] = { 0 };
};

#define RECV_BUFF_SIZE 4096

class TcpServer
{
private:
	SOCKET _sock; // socket�ļ�������
	// -------------------
	// ���ջ�����
	static char _recvBuf[RECV_BUFF_SIZE];
	// �ڶ������� ��Ϣ������
	static char _recvBuf2[RECV_BUFF_SIZE * 10];
	// ��Ϣ������������β��λ��
	static int _lastPos;
	//--------------------
	static set<SOCKET> _cfdsPool;

public:
	TcpServer()
	{
		_sock = INVALID_SOCKET;

		memset(_recvBuf, 0, 1024);
	}
	~TcpServer()
	{
		Close();
	}

	/* ��ʼ�� Socket */
	int Init()
	{
		if (INVALID_SOCKET != _sock)
		{
			Close(); // ��ֹ�������ӷ�����ʧ��
			printf("[sock:%d]Disconnect old connection successfully!\n", _sock);
		}

		// ����Win Socket 2.x����
		#ifdef _MSC_VER // Windows
			WORD ver = MAKEWORD(2, 2);
			WSADATA dat;
			WSAStartup(ver, &dat);
		#endif

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
	static int SendData(SOCKET cSock = INVALID_SOCKET, DataHeader *header = NULL)
	{
		if (cSock != INVALID_SOCKET && header)
		{
			return send(cSock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}

	/* ���͹㲥���� */
	void SendDataAll(DataHeader *header)
	{
		if (header)
		{
			for (set<SOCKET>::iterator it = _cfdsPool.begin(); it != _cfdsPool.end(); it++)
			{
				send(*it, (const char*)header, header->dataLength, 0);
			}
		}
	}

	/* �������� ����ճ������ְ� */
	static void Read_cb(int c_fd, short events, void *user_arg)
	{
		// ���շ�����������
		int recvlen = recv(c_fd, _recvBuf, RECV_BUFF_SIZE, 0);
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
		memcpy(_recvBuf2 + _lastPos, _recvBuf, recvlen);
		_lastPos += recvlen; // ��Ϣ������������β��λ�ú���
		if (_lastPos >= sizeof(DataHeader))
		{
			DataHeader* header = (DataHeader*)_recvBuf2;
			if (_lastPos >= header->dataLength)
			{
				Msg* msg = (Msg*)_recvBuf2;
				cout << "recvlen = " << recvlen << endl;
				cout << msg->data << endl;

				// ��Ӧ���ͻ���
				SendData(c_fd, msg);


				// ʣ��δ������Ϣ���������ݵĳ���
				int nSize = _lastPos - header->dataLength;
				// ����Ϣ������ʣ��δ��������ǰ��
				memcpy(_recvBuf2, _recvBuf2 + header->dataLength, nSize);
				// ��Ϣ������������β��λ��ǰ��
				_lastPos = nSize;
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

			// ����Win Socket 2.x����ռ����Դ
			#ifdef _MSC_VER // Windows
				WSACleanup();
			#endif

			_sock = INVALID_SOCKET;
		}
	}

	/* �۲쵱ǰ�ͻ������������� */
	void Wacth()
	{
		while (true)
		{
			cout << "��ǰ��������:" << _cfdsPool.size() << endl;
			Sleep(1000);
		}
	}

	/* ���Է������� */
	void testSend()
	{
		while (true)
		{
			Msg msg;
			strcpy(msg.data, "Server Echo!!");
			SendDataAll(&msg);
			Sleep(1000);
		}
	}
};

// ���ջ�����
char TcpServer::_recvBuf[RECV_BUFF_SIZE] = { 0 };
// �ڶ������� ��Ϣ������
char TcpServer::_recvBuf2[RECV_BUFF_SIZE * 10] = { 0 };
// ��Ϣ������������β��λ��
int TcpServer::_lastPos = 0;
// �ͻ������ӳ�����
set<SOCKET> TcpServer::_cfdsPool = {};

int main(int argc, char **argv)
{
	int port = 9995;

	
	TcpServer server;
	server.Listen(port, NULL); // �˿�
	thread thr_server(&TcpServer::Start, server),
		thr_wacth(&TcpServer::Wacth, server);
	thr_server.detach();
	thr_wacth.detach();

	cout << "�����������ɹ�!!" << endl;
	
	// �ͻ�����Ϣ���ղ���
	//thread thr_testSend(&TcpServer::testSend, server);
	//thr_testSend.detach();


	getchar();
	return 0;
}