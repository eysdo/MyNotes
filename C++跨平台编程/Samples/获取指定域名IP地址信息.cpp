#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main(int argc, char **argv)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	hostent *host = gethostbyname("www.baidu.com");
	if (!host)
	{
		puts("Get IP address error!");
		getchar();
		return 0;
	}

	//别名
	for (int i = 0; host->h_aliases[i]; i++)
	{
		printf("Aliases %d: %s\n", i + 1, host->h_aliases[i]);
	}

	//地址类型
	printf("Address type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

	//IP地址
	for (int i = 0; host->h_addr_list[i]; i++)
	{
		printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(in_addr*)host->h_addr_list[i]));
	}

	if (host->h_addr)
	{
		cout << inet_ntoa(*(in_addr*)host->h_addr) << endl;
	}
	



	getchar();
	return 0;
}