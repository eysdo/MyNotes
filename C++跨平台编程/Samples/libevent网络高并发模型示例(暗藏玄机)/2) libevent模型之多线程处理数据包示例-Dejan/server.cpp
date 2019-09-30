#ifdef _MSC_VER // Windows
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  // 解决#include <WinSock2.h> 里面有重定义的问题
#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_ntoa()过时问题

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

struct DataHeader
{
	int dataLength;  // 数据长度
};
struct Msg :public DataHeader
{
	Msg()
	{
		dataLength = sizeof(Msg);
	}
	char data[1024] = { 0 };
};

#define RECV_BUFF_SIZE 65535

class TcpServer
{
private:
	SOCKET _sock; // socket文件描述符
	static set<SOCKET> _cfdsPool;
	static queue<DataHeader*> _packet_queue; // 数据包队列
	static mutex _mux; // 互斥锁


public:
	TcpServer()
	{
		_sock = INVALID_SOCKET;

		// 启动Win Socket 2.x环境
		#ifdef _MSC_VER // Windows
				WORD ver = MAKEWORD(2, 2);
				WSADATA dat;
				WSAStartup(ver, &dat);
		#endif
	}
	~TcpServer()
	{
		Close();
		// 回收Win Socket 2.x环境占用资源
		#ifdef _MSC_VER // Windows
				WSACleanup();
		#endif
	}

	/* 初始化 Socket */
	int Init()
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

		// 允许多次绑定同一个地址。要用在socket和bind之间
		evutil_make_listen_socket_reuseable(_sock);
	}

	/* 绑定端IP和口号 */
	void Bind(const unsigned short port, const char* ip = NULL)
	{
		sockaddr_in _sin;
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port); // host to net unsigned short
		_sin.sin_addr.s_addr = NULL == ip ? INADDR_ANY : inet_addr(ip);// INADDR_ANY 同 inet_addr("0.0.0.0");
		if (SOCKET_ERROR == ::bind(_sock, (sockaddr*)&_sin, (socklen_t)sizeof(_sin)))
		{
			throw runtime_error("ERROR:bind(),绑定端口失败!!\n");
		}
	}

	/* 监听端口号 */
	void Listen(int backlog)
	{
		if (SOCKET_ERROR == listen(_sock, backlog))
		{
			throw runtime_error("ERROR:listen(),监听网络端口失败!!\n");
		}
		// 跨平台统一接口，将套接字设置为非阻塞状态
		evutil_make_socket_nonblocking(_sock);
	}

	/* 绑定端IP和口号 同时监听, 一步到位 */
	void Listen(const unsigned short port, const char* ip = NULL, int backlog = 10)
	{
		if (INVALID_SOCKET == _sock)
		{
			Init(); // 如果未初始化连接,自动初始化Socket
		}
		Bind(port, ip);
		Listen(backlog);
	}

	/* 提取客户端连接 */
	static void Accept_cb(int fd, short events, void* user_arg)
	{
		sockaddr_in clienAddr;
		socklen_t addrlen = sizeof(clienAddr);
		SOCKET cSock = INVALID_SOCKET;

		cSock = accept(fd, (sockaddr*)&clienAddr, &addrlen);
		if (INVALID_SOCKET == cSock)
		{
			cout << "ERROR:accept(),接收到无效的客户端SOCKET!!" << endl;
			return;
		}

		// 设置非阻塞
		evutil_make_socket_nonblocking(cSock);

		event_base* base = (event_base*)user_arg;
		//仅仅是为了动态创建一个event结构体
		event *ev = event_new(NULL, -1, 0, NULL, NULL);
		event_assign(ev, base, cSock, EV_READ | EV_PERSIST,
			Read_cb, (void*)ev);

		event_add(ev, NULL);

		_cfdsPool.insert(cSock);
		cout << "新连接客户端IP:" << inet_ntoa(clienAddr.sin_addr)
			<< " 端口:" << clienAddr.sin_port << endl;
	}

	/* 发送指定用户数据 */
	static int SendData(SOCKET cSock = INVALID_SOCKET, DataHeader *header = NULL)
	{
		if (cSock != INVALID_SOCKET && header)
		{
			return send(cSock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}

	/* 发送广播数据 */
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

	/* 接收数据 处理粘包、拆分包 */
	static void Read_cb(int c_fd, short events, void *user_arg)
	{
		// 接收缓冲区
		static char recvBuf[RECV_BUFF_SIZE] = { 0 };
		// 第二缓冲区 消息缓冲区
		static char secondBuf[RECV_BUFF_SIZE * 2500] = { 0 };
		// 消息缓冲区的数据尾部位置
		static int lastPos = 0;

		// 接收服务器端数据
		int recvlen = recv(c_fd, recvBuf, RECV_BUFF_SIZE, 0);
		if (recvlen <= 0)
		{
			cout << "客户端<Socket:" << c_fd << ">已退出, 任务结束!" << endl;
			_cfdsPool.erase(c_fd);
			event_free((event*)user_arg);
			closesocket(c_fd);
			c_fd = INVALID_SOCKET;
			return;
		}

		// 将收取到的数据拷贝到消息缓冲区
		memcpy(secondBuf + lastPos, recvBuf, recvlen);
		lastPos += recvlen; // 消息缓存区的数据尾部位置后移
		if (lastPos >= sizeof(DataHeader))
		{
			DataHeader* header = (DataHeader*)secondBuf;
			if (lastPos >= header->dataLength)
			{
				Msg* msg = (Msg*)secondBuf;
				//cout << "recvlen = " << recvlen << endl;
				//cout << "data" << msg->data << endl;
				
				// 拷贝数据包, 放入数据包队列
				Msg* cpy_msg = (Msg*)malloc(msg->dataLength);
				memcpy(cpy_msg, msg, msg->dataLength);
				_mux.lock();
				_packet_queue.push(cpy_msg);
				_mux.unlock();


				// 剩余未处理消息缓冲区数据的长度
				int nSize = lastPos - header->dataLength;
				// 将消息缓存区剩余未处理数据前移
				memcpy(secondBuf, secondBuf + header->dataLength, nSize);
				// 消息缓冲区的数据尾部位置前移
				lastPos = nSize;
			}
		}
	}

	/* 启动服务 */
	void Start()
	{
		event_base* base = event_base_new();
		event* ev_listen = event_new(base, _sock, EV_READ | EV_PERSIST,
			Accept_cb, base);
		event_add(ev_listen, NULL);

		event_base_dispatch(base); // 阻塞, 进入事件循环..

		event_free(ev_listen);
		event_base_free(base);
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

	/* 观察当前客户端在线连接数 */
	void Wacth()
	{
		while (true)
		{
			cout << "当前在线人数:" << _cfdsPool.size() << endl;
			Sleep(1000);
		}
	}

	/* 数据包处理线程 */
	void packetWork()
	{
		while (true)
		{
			_mux.lock();
			if (!_packet_queue.size())
			{
				_mux.unlock();
				Sleep(5);
				continue;
			}

			Msg* msg = (Msg*)_packet_queue.front();
			cout << "data:" << msg->data << endl;
			free(msg);
			_packet_queue.pop();
			_mux.unlock();
			Sleep(5);
		}
	}
};

// 客户端连接池容器
set<SOCKET> TcpServer::_cfdsPool = {};
queue<DataHeader*> TcpServer::_packet_queue; // 数据包队列
mutex TcpServer::_mux; // 互斥锁

int main(int argc, char **argv)
{
	int port = 9995;

	
	TcpServer server;
	server.Listen(port, NULL); // 端口
	thread thr_server(&TcpServer::Start, server);
	thr_server.detach();

	cout << "服务器启动成功!!" << endl;

	// 开4个工作线程
	int thread_num = 2;
	vector<thread> thrs_working;
	for (int i = 1; i <= thread_num; i++)
	{
		thrs_working.push_back(move(thread(&TcpServer::packetWork, server)));
	}
	for (vector<thread>::iterator it = thrs_working.begin(); it != thrs_working.end(); it++)
	{
		(*it).detach();
	}
	cout << "工作线程启动成功!! 工作线程数:"<< thrs_working.size() << endl;

	getchar();
	return 0;
}