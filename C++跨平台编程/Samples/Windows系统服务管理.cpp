#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
using namespace std;

/*
名称：系统服务管理
语言：C++
介绍：对Windows系统服务的状态获取，服务暂停，开启，停止操作代码
*/

#pragma comment(lib, "advapi32.lib")

int main(int argc, char **argv)
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	SERVICE_STATUS ssSvcStatus = {}; // 服务状态


	/* 打开服务管理对象 */
	schSCManager = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (schSCManager == NULL)
	{
		cout << "open SCManager error!" << endl;
		goto Cleanup;
	}

	/* 获得操作服务句柄 与fopen相似 模: 启动|查询状态|暂停 */
	schService = ::OpenService(schSCManager, L"MtShieldService",
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (schService == NULL)
	{
		cout << "Open service error!" << endl;
		goto Cleanup;
	}

	/* 获得服务状态 */
	if (::QueryServiceStatus(schService, &ssSvcStatus) == FALSE)
	{
		cout << "Get Service state error!" << endl;
		goto Cleanup;
	}

	/* 如果处于停止状态则启动服务，否则停止服务。*/
	if (ssSvcStatus.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务
		if (::ControlService(schService,
			SERVICE_CONTROL_STOP, &ssSvcStatus) == FALSE)
		{
			cout << "stop service error!" << endl;
			goto Cleanup;
		}
		// 等待服务停止
		while (::QueryServiceStatus(schService, &ssSvcStatus) == TRUE)
		{
			::Sleep(ssSvcStatus.dwWaitHint);
			if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
			{
				cout << "stop success!" << endl;
				goto Cleanup;
			}
		}
	}
	else if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
	{
		// 启动服务
		if (::StartService(schService, NULL, NULL) == FALSE)
		{
			cout << "start service error!" << endl;
			goto Cleanup;
		}
		// 等待服务启动
		while (::QueryServiceStatus(schService, &ssSvcStatus) == TRUE)
		{
			::Sleep(ssSvcStatus.dwWaitHint);
			if (ssSvcStatus.dwCurrentState == SERVICE_RUNNING)
			{
				cout << "start success!" << endl;
				goto Cleanup;
			}
		}
	}



Cleanup:
	// 空间释放
	if (schSCManager)
	{
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
	}
	if (schService)
	{
		CloseServiceHandle(schService);
		schService = NULL;
	}

	getchar();
	return 0;
}