#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Injector.h"
using namespace std;
using namespace injector;

int main(int argc, char **argv)
{
	// Ȩ�޻�ȡ�ж�
	if (enableDebugPrivilege() == false)
	{
		return 0;
	}

	wchar_t wzDllFullPath[MAX_PATH] = { 0 }; // Ԥע���DLL�ļ�·��
	#ifdef  _WIN64
		wcsncat_s(wzDllFullPath, L"E:\\Dll64.dll", 20);
	#else
		wcsncat_s(wzDllFullPath, L"../Dll_detours/Dll_detours.dll", wcslen(L"../Dll_detours/Dll_detours.dll"));
	#endif

	bool ret = false;
	// ��ȡ����PID
	vector<unsigned long> pidList = getProcessIDByName(L"detours_test_exe.exe");
	for (vector<unsigned long>::iterator it = pidList.begin(); it != pidList.end(); it++)
	{
		ret = injectDllByRemoteThread(*it, wzDllFullPath);
	}

	if(ret)
		cout << "ע��ɹ�!!" << endl;
	else
		cout << "ע��ʧ��!!" << endl;


	getchar();
	return 0;
}