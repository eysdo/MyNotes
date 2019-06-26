// 简单的测试注入dll, 使用 DebugView 查看调试信息

#include <Windows.h>

void __declspec(dllexport) test()
{
	OutputDebugString(L"我成功注入了!!\r\n");
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		test();
		break;
	}
	case DLL_THREAD_ATTACH:
	{
		//MessageBox(NULL, "DLL_THREAD_ATTACH", "TestDllInject", MB_OK);
		//test();
		break;
	}
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}