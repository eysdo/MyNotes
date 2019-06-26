// Detours Hook 注入dll测试exe

#include <Windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "User32.lib")

void main(void)
{
	cout << "请按下任意键执行弹出MessageBox!!" << endl;
	while (true)
	{
		getchar();
		MessageBox(NULL, TEXT("安装成功!!"), TEXT("提示:"), MB_OK);
	}
	

}