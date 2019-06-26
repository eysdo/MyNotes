#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Injector.h"
using namespace std;
using namespace injector;

int main(int argc, char **argv)
{
	// 权限获取判断
	if (enableDebugPrivilege() == false)
	{
		return 0;
	}

	wchar_t wzDllFullPath[MAX_PATH] = { 0 }; // 预注入的DLL文件路径
	#ifdef  _WIN64
		wcsncat_s(wzDllFullPath, L"E:\\Dll64.dll", 20);
	#else
		wcsncat_s(wzDllFullPath, L"../Dll_detours/Dll_detours.dll", wcslen(L"../Dll_detours/Dll_detours.dll"));
	#endif

	bool ret = false;
	// 获取进程PID
	vector<unsigned long> pidList = getProcessIDByName(L"detours_test_exe.exe");
	for (vector<unsigned long>::iterator it = pidList.begin(); it != pidList.end(); it++)
	{
		ret = injectDllByRemoteThread(*it, wzDllFullPath);
	}

	if(ret)
		cout << "注入成功!!" << endl;
	else
		cout << "注入失败!!" << endl;


	getchar();
	return 0;
}