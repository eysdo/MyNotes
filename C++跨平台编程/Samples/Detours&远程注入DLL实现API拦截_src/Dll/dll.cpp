// �򵥵Ĳ���ע��dll, ʹ�� DebugView �鿴������Ϣ

#include <Windows.h>

void __declspec(dllexport) test()
{
	OutputDebugString(L"�ҳɹ�ע����!!\r\n");
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