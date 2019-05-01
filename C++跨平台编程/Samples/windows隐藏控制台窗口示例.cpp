#include <windows.h>
#include <stdio.h>

int main(void)
{
	HWND hWnd = GetForegroundWindow();
	ShowWindow(hWnd, 0); //隐藏窗口
	Sleep(3000); // 3秒后显示
	ShowWindow(hWnd, 1); // 显示窗口

	printf("hello!!");

	getchar();
	return 0;
}