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

class TcpClient
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

public:
	TcpClient()
	{
		_sock = INVALID_SOCKET;
	}
	~TcpClient()
	{
		Close();
	}

	/* ��ʼ�� socket */
	void Init()
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

	/* ���ӷ����� */
	void Connect(const char* ip, unsigned short port)
	{
		if (INVALID_SOCKET == _sock)
		{
			Init(); // ���δ��ʼ������,�Զ���ʼ��Socket
		}

		sockaddr_in _sin;
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);
		_sin.sin_addr.s_addr = inet_addr(ip);
		if (SOCKET_ERROR == connect(_sock, (sockaddr*)&_sin, sizeof(_sin)))
		{
			throw std::runtime_error("ERROR:connect(),Connect server failed!!\n");
		}
	}

	/* �������� ����ճ���������ְ� */
	/* �������� ����ճ������ְ� */
	int RecvData()
	{
		// 5 ���շ�����������
		int recvlen = recv(_sock, _recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "��������Ͽ�����, �������!" << endl;
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

	/* �������� */
	int SendData(DataHeader *header = NULL)
	{
		if (header)
		{
			return send(_sock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
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
		TcpClient client;
		client.Connect("127.0.0.1", 9000);
		while (true)
		{
			//Msg msg;
			//strcpy(msg.data, "hello dejan!!");
			//client.SendData(&msg);
			//Sleep(1);

			client.RecvData();
		}
		client.Close();
	}
	catch (const exception& e)
	{
		cerr << "Get exception : " << e.what() << endl;
	}
	

	getchar();
	return 0;
}