#include "TcpClient.hpp"

int main(int argc, char **argv)
{
	const char* ip = "127.0.0.1";
	int port = 9995;

	TcpClient client;
	thread thr_client(&TcpClient::Connect, client, ip, port);
	thr_client.detach(); // ���ӶϿ����Զ�������..

	while (true)
	{
		//Sleep(100);
		client.SendData("Hello!!");
	}
	


	getchar();
	return 0;
}