#include "TCPServer.hpp"

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