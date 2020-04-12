#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

enum WindowsFlags
{
	WIN_MIN = 0x00000001,   // ��С�� ��
	WIN_MAX = 0x00000002,   // ��� ��
	WIN_CLOSE = 0x00000004, // �ر�   ��
	WIN_MIN_AND_CLOSE = WIN_MIN | WIN_CLOSE, // ����
	WIN_STANDARD = WIN_MIN_AND_CLOSE | WIN_MAX, // ������
};
/*
use: flag |= WIN_MAX // ��� ��� ��
	 flag ^= WIN_MAX // ɾ�� ��� ��
	 if((flag & WIN_MAX) == WIN_MAX) // �ж�flag�Ƿ��� ��󻯡�(Has)
*/

/* ��ʾ���� */
void ShowWindow(int flag)
{
	// ��С�� ��
	if ((flag & WIN_MIN) == WIN_MIN)
	{
		cout << "��";
	}
	// ��� ��
	if ((flag & WIN_MAX) == WIN_MAX)
	{
		cout << "��";
	}
	// �ر� ��
	if ((flag & WIN_CLOSE) == WIN_CLOSE)
	{
		cout << "��";
	}
	cout << endl;
}

int main()
{
	// ���� 3�����ʹ���

	// ��
	ShowWindow(WIN_CLOSE);

	// ������
	cout << "\n=======================================" << endl;
	int flags = WIN_MAX;
	flags |= WIN_MIN_AND_CLOSE;
	ShowWindow(flags);
	ShowWindow(WIN_MIN | WIN_MAX | WIN_CLOSE);
	ShowWindow(WIN_STANDARD);

	// ����
	cout << "\n=======================================" << endl;
	ShowWindow(WIN_MIN | WIN_CLOSE);
	ShowWindow(WIN_STANDARD^WIN_MAX);
	ShowWindow(WIN_MIN_AND_CLOSE);





	getchar();
	return 0;
}

