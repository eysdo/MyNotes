#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

// C++11 ģ�庯�����ػ�


template <typename T>
void func(T i)
{
	cout << "һ��ģ��: " << i << endl;
}

template <>
void func(int i)
{
	cout << "ģ���ػ�: " << i << endl;
}


int main(int argc, char **argv)
{
	int i = 10;
	func(i); //�����ػ��汾


	getchar();
	return 0;
}