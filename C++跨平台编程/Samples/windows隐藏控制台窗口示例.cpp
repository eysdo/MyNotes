#include <windows.h>
#include <stdio.h>

int main(void)
{
	HWND hWnd = GetForegroundWindow();
	ShowWindow(hWnd, 0); //���ش���
	Sleep(3000); // 3�����ʾ
	ShowWindow(hWnd, 1); // ��ʾ����

	printf("hello!!");

	getchar();
	return 0;
}