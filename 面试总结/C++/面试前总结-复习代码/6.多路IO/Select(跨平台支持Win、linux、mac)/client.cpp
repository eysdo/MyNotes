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

#define RECV_BUFF_SIZE 1024

class TcpClient
{
private:
	SOCKET _sock; // socket文件描述符
	// -------------------
	// 接收缓冲区
	char _recvBuf[RECV_BUFF_SIZE] = { 0 };
	// 第二缓冲区 消息缓冲区
	char _recvBuf2[RECV_BUFF_SIZE * 10] = { 0 };
	// 消息缓冲区的数据尾部位置
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

	/* 初始化 socket */
	void Init()
	{
		if (INVALID_SOCKET != _sock)
		{
			Close(); // 防止重新连接服务器失败
			printf("[sock:%d]Disconnect old connection successfully!\n", _sock);
		}

		// 启动Win Socket 2.x环境
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

	/* 连接服务器 */
	void Connect(const char* ip, unsigned short port)
	{
		if (INVALID_SOCKET == _sock)
		{
			Init(); // 如果未初始化连接,自动初始化Socket
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

	/* 接收数据 处理粘包、处理拆分包 */
	/* 接收数据 处理粘包、拆分包 */
	int RecvData()
	{
		// 5 接收服务器端数据
		int recvlen = recv(_sock, _recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "与服务器断开连接, 任务结束!" << endl;
			return -1;
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

		return 0;
	}

	/* 发送数据 */
	int SendData(DataHeader *header = NULL)
	{
		if (header)
		{
			return send(_sock, (const char*)header, header->dataLength, 0);
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

			// 回收Win Socket 2.x环境占用资源
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