#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
using namespace std;

/*
���ƣ�ϵͳ�������
���ԣ�C++
���ܣ���Windowsϵͳ�����״̬��ȡ��������ͣ��������ֹͣ��������
*/

#pragma comment(lib, "advapi32.lib")

int main(int argc, char **argv)
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	SERVICE_STATUS ssSvcStatus = {}; // ����״̬


	/* �򿪷��������� */
	schSCManager = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (schSCManager == NULL)
	{
		cout << "open SCManager error!" << endl;
		goto Cleanup;
	}

	/* ��ò��������� ��fopen���� ģ: ����|��ѯ״̬|��ͣ */
	schService = ::OpenService(schSCManager, L"MtShieldService",
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (schService == NULL)
	{
		cout << "Open service error!" << endl;
		goto Cleanup;
	}

	/* ��÷���״̬ */
	if (::QueryServiceStatus(schService, &ssSvcStatus) == FALSE)
	{
		cout << "Get Service state error!" << endl;
		goto Cleanup;
	}

	/* �������ֹͣ״̬���������񣬷���ֹͣ����*/
	if (ssSvcStatus.dwCurrentState == SERVICE_RUNNING)
	{
		// ֹͣ����
		if (::ControlService(schService,
			SERVICE_CONTROL_STOP, &ssSvcStatus) == FALSE)
		{
			cout << "stop service error!" << endl;
			goto Cleanup;
		}
		// �ȴ�����ֹͣ
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
		// ��������
		if (::StartService(schService, NULL, NULL) == FALSE)
		{
			cout << "start service error!" << endl;
			goto Cleanup;
		}
		// �ȴ���������
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
	// �ռ��ͷ�
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