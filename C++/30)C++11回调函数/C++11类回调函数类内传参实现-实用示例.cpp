#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <queue>
using namespace std;

class EchoServer
{
	class MsgType
	{
	public:
		int _sock = 0;
		string _data;
	public:
		MsgType(int sock, string data) 
			:_sock(sock), _data(data)
		{}
	};
	using self = EchoServer;
	typedef function<void(int, string)> callback_recv;
private:
	short _thread_num = 1;
	queue<MsgType> _recv_data;
	callback_recv _recv_func;
	mutex _mtx;
	condition_variable _cv;
public:
	self* multithreaded(short thread_num = 4)
	{
		_thread_num = thread_num;
		return this;
	}
	self* recv(callback_recv func)
	{
		_recv_func = move(func);
		return this;
	}
	bool send(const string& data, int sock = 123)
	{
		unique_lock<mutex> lock(_mtx);
		_recv_data.push(move(MsgType(sock, data)));
		_cv.notify_one();
		return true;
	}
	void start()
	{
		for (size_t i = 0; i < _thread_num; i++)
		{
			thread([&] {
				while (true)
				{
					{
						unique_lock<mutex> lock(_mtx);
						while (_recv_data.empty())
							_cv.wait(lock);
						_recv_func(_recv_data.front()._sock, _recv_data.front()._data);
						_recv_data.pop();
					}
					this_thread::sleep_for(chrono::microseconds(1));
				}
			}).detach();
		}
	}
};

int main(int argc, char **argv)
{
	EchoServer server;
	server.recv([](int sock, string data){
		cout << "sock:" << sock << " data:" << data << endl;
	})
	->multithreaded()
	->start();

	server.send("Hello World!");
	server.send("Dejan");
	server.send("Dejan");
	for (size_t i = 0; i < 10000; i++)
	{
		server.send("Dejan", i);
	}


	getchar();
	return 0;
}