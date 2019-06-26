#include "Injector.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <tchar.h>
using namespace std;

namespace injector
{
	class CRemoteThread : public RemoteThread
	{
	public:
		/* 通过进程名获取进程ID */
		vector<unsigned long> GetProcessIDByName(const WCHAR* pName)
		{
			vector<unsigned long> pidList;
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (INVALID_HANDLE_VALUE == hSnapshot) {
				return{};
			}
			PROCESSENTRY32 pe = { sizeof(pe) };
			for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
				if (wcscmp(pe.szExeFile, pName) == 0) {
					pidList.push_back(pe.th32ProcessID);
				}
				//printf("%-6d %s\n", pe.th32ProcessID, pe.szExeFile);
			}
			CloseHandle(hSnapshot);
			return pidList;
		}

		/* 启用调试权限 */
		bool EnableDebugPrivilege()
		{
			HANDLE TokenHandle = NULL;
			TOKEN_PRIVILEGES TokenPrivilege;
			LUID uID;
			//打开权限令牌
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))
			{
				return FALSE;
			}
			if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &uID))
			{
				CloseHandle(TokenHandle);
				TokenHandle = INVALID_HANDLE_VALUE;
				return FALSE;
			}
			TokenPrivilege.PrivilegeCount = 1;
			TokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			TokenPrivilege.Privileges[0].Luid = uID;
			if (!AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivilege, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
				//调整权限
			{
				CloseHandle(TokenHandle);
				TokenHandle = INVALID_HANDLE_VALUE;
				return  FALSE;
			}
			CloseHandle(TokenHandle);
			TokenHandle = INVALID_HANDLE_VALUE;
			return TRUE;
		}

		/* 远程注入DLL到进程 */
		bool InjectDllByRemoteThread(ULONG32 ulTargetProcessID, WCHAR* wzDllFullPath)
		{
			HANDLE  TargetProcessHandle = NULL;
			TargetProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ulTargetProcessID);
			if (NULL == TargetProcessHandle)
			{
				printf("failed to open process!!\n");
				return FALSE;
			}
			WCHAR* VirtualAddress = NULL;
			ULONG32 ulDllLength = (ULONG32)_tcslen(wzDllFullPath) + 1;
			//ALLOC Address for Dllpath
			VirtualAddress = (WCHAR*)VirtualAllocEx(TargetProcessHandle, NULL, ulDllLength * sizeof(WCHAR), MEM_COMMIT, PAGE_READWRITE);
			if (NULL == VirtualAddress)
			{
				printf("failed to Alloc!!\n");
				CloseHandle(TargetProcessHandle);
				return FALSE;
			}
			// write
			if (FALSE == WriteProcessMemory(TargetProcessHandle, VirtualAddress, (LPVOID)wzDllFullPath, ulDllLength * sizeof(WCHAR), NULL))
			{
				printf("failed to write!!\n");
				VirtualFreeEx(TargetProcessHandle, VirtualAddress, ulDllLength, MEM_DECOMMIT);
				CloseHandle(TargetProcessHandle);
				return FALSE;
			}
			LPTHREAD_START_ROUTINE FunctionAddress = NULL;
			FunctionAddress = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryW");
			HANDLE ThreadHandle = INVALID_HANDLE_VALUE;
			//start
			ThreadHandle = CreateRemoteThread(TargetProcessHandle, NULL, 0, FunctionAddress, VirtualAddress, 0, NULL);
			if (NULL == ThreadHandle)
			{
				VirtualFreeEx(TargetProcessHandle, VirtualAddress, ulDllLength, MEM_DECOMMIT);
				CloseHandle(TargetProcessHandle);
				return FALSE;
			}
			// WaitForSingleObject
			WaitForSingleObject(ThreadHandle, INFINITE);
			VirtualFreeEx(TargetProcessHandle, VirtualAddress, ulDllLength, MEM_DECOMMIT);			// 清理
			CloseHandle(ThreadHandle);
			CloseHandle(TargetProcessHandle);
		}
	};


	/* 构造函数 */
	RemoteThread* RemoteThread::Get()
	{
		static RemoteThread* rt = 0;
		if (0 == rt)
		{
			static CRemoteThread c_rt;
			rt = &c_rt;
		}
		return rt;
	}


}
