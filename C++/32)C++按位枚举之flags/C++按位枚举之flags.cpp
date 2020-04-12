#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

enum WindowsFlags
{
	WIN_MIN = 0x00000001,   // 最小化 －
	WIN_MAX = 0x00000002,   // 最大化 □
	WIN_CLOSE = 0x00000004, // 关闭   ×
	WIN_MIN_AND_CLOSE = WIN_MIN | WIN_CLOSE, // －×
	WIN_STANDARD = WIN_MIN_AND_CLOSE | WIN_MAX, // －□×
};
/*
use: flag |= WIN_MAX // 添加 最大化 □
	 flag ^= WIN_MAX // 删除 最大化 □
	 if((flag & WIN_MAX) == WIN_MAX) // 判断flag是否有 最大化□(Has)
*/

/* 显示窗口 */
void ShowWindow(int flag)
{
	// 最小化 －
	if ((flag & WIN_MIN) == WIN_MIN)
	{
		cout << "－";
	}
	// 最大化 □
	if ((flag & WIN_MAX) == WIN_MAX)
	{
		cout << "□";
	}
	// 关闭 ×
	if ((flag & WIN_CLOSE) == WIN_CLOSE)
	{
		cout << "×";
	}
	cout << endl;
}

int main()
{
	// 例举 3种类型窗口

	// ×
	ShowWindow(WIN_CLOSE);

	// －□×
	cout << "\n=======================================" << endl;
	int flags = WIN_MAX;
	flags |= WIN_MIN_AND_CLOSE;
	ShowWindow(flags);
	ShowWindow(WIN_MIN | WIN_MAX | WIN_CLOSE);
	ShowWindow(WIN_STANDARD);

	// －×
	cout << "\n=======================================" << endl;
	ShowWindow(WIN_MIN | WIN_CLOSE);
	ShowWindow(WIN_STANDARD^WIN_MAX);
	ShowWindow(WIN_MIN_AND_CLOSE);





	getchar();
	return 0;
}

