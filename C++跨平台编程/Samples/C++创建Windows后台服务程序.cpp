#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

#pragma comment(lib, "Advapi32.lib")

#define SERVICE_NAME L"service_demo"
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle;

void WINAPI ServiceHandler(DWORD fdwControl)
{
	switch (fdwControl)
	{
		case SERVICE_CONTROL_STOP:
		case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;

		//add you quit code here

		break;
	default:
		return;
	};
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}
}

void WINAPI service_main(int argc, char** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	hServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceHandler);
	if (hServiceStatusHandle == 0)
	{
		DWORD nError = GetLastError();
	}
	//add your init code here

	//add your service thread here

	// - 测试代码 Start
	// sc create atest binPath= "D:\Project Files\ImosServer\x64\Release  -port=1024"
	// sc start atest -port=1024
	FILE* log = fopen("D:\\log.txt", "a");
	for (int i = 0; i < argc; ++i)
	{
		fprintf(log, "main: %s\n", argv[i]);
	}
	fclose(log);
	// - 测试代码 End


	// Initialization complete - report running status 
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 9000;
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}

}

int main(int argc, char **argv)
{
	SERVICE_TABLE_ENTRY ServiceTable[2];

	// lpServiceName: 指向表示服务名称字符串的指针；当定义了多个服务时，那么这个域必须指定
	// lpServiceProc: 指向服务主函数的指针（服务入口点）
	ServiceTable[0].lpServiceName = SERVICE_NAME;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)service_main;

	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	
	// 启动服务的控制分派机线程
	StartServiceCtrlDispatcher(ServiceTable);

	return 0;
}