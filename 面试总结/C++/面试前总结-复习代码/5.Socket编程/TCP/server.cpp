#ifdef _MSC_VER // Windows
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  // ���#include <WinSock2.h> �������ض��������
#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_ntoa()��ʱ����

#include <Windows.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#define socklen_t int

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
	SOCKET _cSock;
	// -------------------
	// ���ջ�����
	char _recvBuf[RECV_BUFF_SIZE] = { 0 };
	// �ڶ������� ��Ϣ������
	char _recvBuf2[RECV_BUFF_SIZE * 10] = { 0 };
	// ��Ϣ������������β��λ��
	int _lastPos = 0;

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
		_cSock = INVALID_SOCKET;

		_cSock = accept(_sock, (sockaddr*)&clienAddr, &addrlen);
		if (INVALID_SOCKET == _cSock)
		{
			cout << "ERROR:accept(),���յ���Ч�Ŀͻ���SOCKET!!" << endl;
		}
		else
		{
			cout << "�����ӿͻ���IP:" << inet_ntoa(clienAddr.sin_addr)
				<< " �˿�:" << clienAddr.sin_port << endl;
		}
	}

	/* ����ָ���û����� */
	int SendData(DataHeader *header = NULL)
	{
		if (header)
		{
			return send(_cSock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}

	/* �������� ����ճ������ְ� */
	int RecvData()
	{
		// 5 ���շ�����������
		int recvlen = recv(_cSock, _recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "�ͻ���<Socket:" << _cSock << ">���˳�, �������!" << endl;
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
		TcpServer server;
		server.Listen(9000, NULL); // �˿� 9000
		server.Accept();
		while (true)
		{
			//server.RecvData();

			Msg msg;
			strcpy(msg.data, "server echo!!");
			server.SendData(&msg);
			Sleep(1);
		}
		
	}
	catch (const exception& e)
	{
		cerr << "Get exception : " << e.what() << endl;
	}
	 


	getchar();
	return 0;
}