#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<thread>
#include<string>
using namespace std;

void ThreadFunc(string& str)
{
	cout << str << endl;
}

int main()
{
	thread t1;
	cout << t1.get_id() << endl;   // 0, �߳�ID:0
	cout << t1.joinable() << endl; // 0, ���ɱ�join()

	// ʹ���вι��캯��
	thread t2(ThreadFunc, string("Hello"));
	cout << t2.get_id() << endl;
	cout << t2.joinable() << endl;

	// thread����������֮ǰ�������join(),���������ֹ �� ʹ�� detach()
	//t2.join(); // �ȴ����߳�ִ�����
	t2.detach(); // ʹ��ǰ�̶߳������

	system("pause");
	return 0;
}