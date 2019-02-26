#ifdef _MSC_VER // Windows
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  // ���#include <WinSock2.h> �������ض��������
#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_ntoa()��ʱ����

#include <Windows.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#define socklen_t int
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

#define RECV_BUFF_SIZE 1024

class TcpServer
{
private:
	SOCKET _sock; // socket�ļ�������
	// -------------------
	// ���ջ�����
	char _recvBuf[RECV_BUFF_SIZE] = { 0 };
	// �ڶ������� ��Ϣ������
	char _recvBuf2[RECV_BUFF_SIZE * 10] = { 0 };
	// ��Ϣ������������β��λ��
	int _lastPos = 0;
	//--------------------
	set<SOCKET> _clientFds;

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
	}

	/* �󶨶�IP�Ϳں� ͬʱ����, һ����λ */
	void Listen(const unsigned short port, const char* ip = NULL, int backlog = 5)
	{
		if (INVALID_SOCKET == _sock)
		{
			Init(); // ���δ��ʼ������,�Զ���ʼ��Socket
		}
		Bind(port, ip);
		Listen(backlog);
	}

	/* ��ȡ�ͻ������� */
	void Accept()
	{
		sockaddr_in clienAddr;
		socklen_t addrlen = sizeof(clienAddr);
		SOCKET cSock = INVALID_SOCKET;

		cSock = accept(_sock, (sockaddr*)&clienAddr, &addrlen);
		if (INVALID_SOCKET == cSock)
		{
			cout << "ERROR:accept(),���յ���Ч�Ŀͻ���SOCKET!!" << endl;
		}
		else
		{
			_clientFds.insert(cSock);
			cout << "�����ӿͻ���IP:" << inet_ntoa(clienAddr.sin_addr)
				<< " �˿�:" << clienAddr.sin_port << endl;
		}
	}

	/* ����ָ���û����� */
	int SendData(SOCKET cSock = INVALID_SOCKET, DataHeader *header = NULL)
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
			for (set<SOCKET>::iterator it = _clientFds.begin(); it != _clientFds.end(); it++)
			{
				send(*it, (const char*)header, header->dataLength, 0);
			}
		}
	}

	/* �������� ����ճ������ְ� */
	int RecvData(SOCKET cSock)
	{
		// ���շ�����������
		int recvlen = recv(cSock, _recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "�ͻ���<Socket:" << cSock << ">���˳�, �������!" << endl;
			return -1;
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

				// ʣ��δ������Ϣ���������ݵĳ���
				int nSize = _lastPos - header->dataLength;
				// ����Ϣ������ʣ��δ��������ǰ��
				memcpy(_recvBuf2, _recvBuf2 + header->dataLength, nSize);
				// ��Ϣ������������β��λ��ǰ��
				_lastPos = nSize;
			}
		}
		
		return 0;
	}

	/* ����������Ϣ - selectģ�� */
	bool Start()
	{
		// ������ BSD socket
		fd_set fdRead; // ������(socket) ����
		fd_set fdWrite;
		fd_set fdExp;
		// ����
		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExp);
		// ��������(socket)���뼯��
		FD_SET(_sock, &fdRead);
		FD_SET(_sock, &fdWrite);
		FD_SET(_sock, &fdExp);
		for (set<SOCKET>::iterator it = _clientFds.begin(); it != _clientFds.end(); it++)
		{
			FD_SET(*it, &fdRead);
		}
		set<SOCKET>::iterator maxSock_it = _clientFds.end();
		SOCKET maxSock = _clientFds.empty() ? _sock : *(--maxSock_it); // �������������
		timeval t = { 1, 0 }; // ���� select ������, NULL �� ������
		// nfds ��һ������ֵ, ��ָfd_set����������������(socket)�ķ�Χ, ����������
		// ���������ļ����������ֵ+1, ��Windows�������������д0
		int ret = select(maxSock + 1, &fdRead, &fdWrite, &fdExp, &t); // ��windows��sock ���Բ���1, ����Linux��MacOS���������1����ղ������������ص�����, �����select�ڲ�ͬƽ̨��Ĳ���
		if (ret < 0)
		{
			cout << "ERROR:select(),�������!\n" << endl;
			Close();
			return false;
		}

		if (FD_ISSET(_sock, &fdRead))
		{
			FD_CLR(_sock, &fdRead); // ����
			Accept(); // accept �ȴ��ͻ�������
		}

		for (set<SOCKET>::iterator it = _clientFds.begin(); it != _clientFds.end(); it++)
		{
			if (FD_ISSET(*it, &fdRead))
			{
				if (-1 == RecvData(*it))
				{
					it = _clientFds.erase(it);
					if(it == _clientFds.end())
					{ 
						break;
					}
					continue;
				}
			}
		}
		cout << "��ǰ��������:" << _clientFds.size() << endl;
		return true;
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
};

int main(int argc, char **argv)
{
	try
	{
		int port = 9000;
		if (argc > 1) port = atoi(argv[1]);

		TcpServer server;
		server.Listen(port, NULL); // �˿� 9000
		while (server.Start())
		{
			Msg msg;
			strcpy(msg.data, "server echo!!");
			server.SendDataAll(&msg);
			sleep(1);

		}
		server.Close();
	}
	catch (const exception& e)
	{
		cerr << "Get exception : " << e.what() << endl;
	}
	 


	getchar();
	return 0;
}