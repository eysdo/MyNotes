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

class TcpClient
{
private:
	static SOCKET _sock; // socket�ļ�������
	// -------------------
	// ���ջ�����
	static char _recvBuf[RECV_BUFF_SIZE];
	// �ڶ������� ��Ϣ������
	static char _recvBuf2[RECV_BUFF_SIZE * 10];
	// ��Ϣ������������β��λ��
	static int _lastPos;

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
		// 5 ���շ�����������
		int recvlen = recv(fd, _recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "��������Ͽ�����, �������!" << endl;
			// �˳�event�¼�ѭ��
			event_base_loopbreak((event_base*)user_arg); // �Ͽ��Զ���������
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
	int SendData(DataHeader *header = NULL)
	{
		
		if (header)
		{
			cout << "sockfd = " << _sock << endl;
			return ::send(_sock, (const char*)header, header->dataLength, 0);
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

			_sock = INVALID_SOCKET;
		}
	}

	/* ���Է������� */
	void testSend()
	{
		while (true)
		{
			Msg msg;
			strcpy(msg.data, "Server Echo!!");
			cout<<SendData(&msg)<<endl;
			//cout << WSAGetLastError() << endl; // ����鿴
			Sleep(1000);
		}
	}
};

// socket�ļ�������
SOCKET TcpClient::_sock = INVALID_SOCKET;
// ���ջ�����
char TcpClient::_recvBuf[RECV_BUFF_SIZE] = { 0 };
// �ڶ������� ��Ϣ������
char TcpClient::_recvBuf2[RECV_BUFF_SIZE * 10] = { 0 };
// ��Ϣ������������β��λ��
int TcpClient::_lastPos = 0;

int main(int argc, char **argv)
{
	const char* ip = "127.0.0.1";
	int port = 9995;

	TcpClient client;
	thread thr_client(&TcpClient::Connect, client, ip, port);
	thr_client.detach(); // ���ӶϿ����Զ�������..

	// �ͻ�����Ϣ���ղ���
	thread thr_testSend(&TcpClient::testSend, client);
	thr_testSend.detach();

	getchar();
	return 0;
}