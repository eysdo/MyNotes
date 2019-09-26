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

struct DataHeader
{
	short dataLength;  // 数据长度
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
	static SOCKET _sock; // socket文件描述符
	// -------------------
	// 接收缓冲区
	static char _recvBuf[RECV_BUFF_SIZE];
	// 第二缓冲区 消息缓冲区
	static char _recvBuf2[RECV_BUFF_SIZE * 10];
	// 消息缓冲区的数据尾部位置
	static int _lastPos;

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
		// 5 接收服务器端数据
		int recvlen = recv(fd, _recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "与服务器断开连接, 任务结束!" << endl;
			// 退出event事件循环
			event_base_loopbreak((event_base*)user_arg); // 断开自动重连机制
			return;
		}

		// 将收取到的数据拷贝到消息缓冲区
		memcpy(_recvBuf2 + _lastPos, _recvBuf, recvlen);
		_lastPos += recvlen; // 消息缓存区的数据尾部位置后移
		if (_lastPos >= sizeof(DataHeader))
		{
			DataHeader* header = (DataHeader*)_recvBuf2;
			if (_lastPos >= header->dataLength)
			{
				Msg* msg = (Msg*)_recvBuf2;
				cout << "recvlen = " << recvlen << endl;
				cout << msg->data << endl;

				// 剩余未处理消息缓冲区数据的长度
				int nSize = _lastPos - header->dataLength;
				// 将消息缓存区剩余未处理数据前移
				memcpy(_recvBuf2, _recvBuf2 + header->dataLength, nSize);
				// 消息缓冲区的数据尾部位置前移
				_lastPos = nSize;
			}
		}
	}

	/* 发送数据 */
	int SendData(DataHeader *header = NULL)
	{
		
		if (header)
		{
			cout << "sockfd = " << _sock << endl;
			return ::send(_sock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
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

	/* 测试发送数据 */
	void testSend()
	{
		while (true)
		{
			Msg msg;
			strcpy(msg.data, "Server Echo!!");
			cout<<SendData(&msg)<<endl;
			//cout << WSAGetLastError() << endl; // 错误查看
			Sleep(1000);
		}
	}
};

// socket文件描述符
SOCKET TcpClient::_sock = INVALID_SOCKET;
// 接收缓冲区
char TcpClient::_recvBuf[RECV_BUFF_SIZE] = { 0 };
// 第二缓冲区 消息缓冲区
char TcpClient::_recvBuf2[RECV_BUFF_SIZE * 10] = { 0 };
// 消息缓冲区的数据尾部位置
int TcpClient::_lastPos = 0;

int main(int argc, char **argv)
{
	const char* ip = "127.0.0.1";
	int port = 9995;

	TcpClient client;
	thread thr_client(&TcpClient::Connect, client, ip, port);
	thr_client.detach(); // 连接断开会自动重连接..

	// 客户端消息接收测试
	thread thr_testSend(&TcpClient::testSend, client);
	thr_testSend.detach();

	getchar();
	return 0;
}