// 简单的Detours Hook拦截实现

#include <Windows.h>
#include <stdio.h>
#include <Detours/detours.h>

#ifdef _WIN64
#pragma comment(lib, "detours_x64.lib")
#else
#pragma comment(lib, "detours_x86.lib")
#endif // _WIN64


PVOID g_pOldMessageBoxW = NULL;
PVOID g_pOldMessageBoxA = NULL;

typedef int (WINAPI *PfuncMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
typedef int (WINAPI *PfuncMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);


int WINAPI MyMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	return ((PfuncMessageBoxA)g_pOldMessageBoxA)(hWnd, "Hook This!", "My hook", uType);
}

int WINAPI MyMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	return ((PfuncMessageBoxW)g_pOldMessageBoxW)(hWnd, L"Hook This!", L"My hook", uType);
}

BOOL APIENTRY InstallHook();   // 安装钩子
BOOL APIENTRY UnInstallHook(); // 卸载钩子

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(L"---- DLL_PROCESS_ATTACH ----\n");
		OutputDebugString(L"注入进程 安装Hook成功!!\n");
		OutputDebugString(L"---- DLL_PROCESS_ATTACH ----\n");
		InstallHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		OutputDebugString(L"---- DLL_PROCESS_DETACH ----\n");
		OutputDebugString(L"卸载Hook成功!!\n");
		OutputDebugString(L"---- DLL_PROCESS_DETACH ----\n");
		UnInstallHook();
		break;
	}
	return TRUE;
}

// 安装钩子
BOOL APIENTRY InstallHook()
{
	LONG ret = DetourTransactionBegin();
	if (ret != NO_ERROR)
		OutputDebugString(L"InstallHook() DetourTransactionBegin Error\r\n");
	ret = DetourUpdateThread(GetCurrentThread());
	if (ret != NO_ERROR)
		OutputDebugString(L"InstallHook() DetourUpdateThread Error\r\n");
	g_pOldMessageBoxA = DetourFindFunction("User32.dll", "MessageBoxA");
	g_pOldMessageBoxW = DetourFindFunction("User32.dll", "MessageBoxW");
	ret = DetourAttach(&g_pOldMessageBoxA, MyMessageBoxA);
	if (ret != NO_ERROR)
		OutputDebugString(L"InstallHook() DetourAttach Error\r\n");
	ret = DetourAttach(&g_pOldMessageBoxW, MyMessageBoxW);
	if (ret != NO_ERROR)
		OutputDebugString(L"InstallHook() DetourAttach Error\r\n");
	ret = DetourTransactionCommit();
	return ret == NO_ERROR;
}

// 卸载钩子
BOOL APIENTRY UnInstallHook()
{
	LONG ret = DetourTransactionBegin();
	if (ret != NO_ERROR)
		OutputDebugString(L"UnInstallHook() DetourTransactionBegin Error\r\n");
	ret = DetourUpdateThread(GetCurrentThread());
	if (ret != NO_ERROR)
		OutputDebugString(L"UnInstallHook() DetourUpdateThread Error\r\n");
	ret = DetourDetach(&g_pOldMessageBoxA, MyMessageBoxA);
	if (ret != NO_ERROR)
		OutputDebugString(L"UnInstallHook() DetourDetach Error\r\n");
	ret = DetourDetach(&g_pOldMessageBoxW, MyMessageBoxW);
	if (ret != NO_ERROR)
		OutputDebugString(L"UnInstallHook() DetourDetach Error\r\n");
	ret = DetourTransactionCommit();
	return ret == NO_ERROR;
}
