// Detours Hook ע��dll����exe

#include <Windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "User32.lib")

void main(void)
{
	cout << "�밴�������ִ�е���MessageBox!!" << endl;
	while (true)
	{
		getchar();
		MessageBox(NULL, TEXT("��װ�ɹ�!!"), TEXT("��ʾ:"), MB_OK);
	}
	

}