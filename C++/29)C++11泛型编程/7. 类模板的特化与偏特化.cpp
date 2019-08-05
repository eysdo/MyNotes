#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


// C++11 ��ģ����ػ���ƫ�ػ�


template<typename T>
class CTemplate
{
public:
	CTemplate()
	{
		cout << "һ��ģ��" << endl;
	}
};

template<>
class CTemplate<int>
{
public:
	CTemplate()
	{
		cout << "ģ���ػ�" << endl;
	}
};

template<typename T>
class CTemplate<T*>
{
public:
	CTemplate()
	{
		cout << "ģ��ƫ�ػ� 1" << endl;
	}
};

template<typename T, typename U> // ����һ��ƫ�ػ�
class CTemplate<T(U)>
{
public:
	CTemplate()
	{
		cout << "-------------------------" << endl;
		cout << "ģ��ƫ�ػ� 2" << endl;
		cout << typeid(T).name() << endl;
		cout << typeid(U).name() << endl;
		cout << "-------------------------" << endl;
	}
};

int func(int i)
{
	return 2 * i;
}

int main(int argc, char **argv)
{
	CTemplate<float> t1; // һ��ģ��
	CTemplate<int> t2; // ģ���ػ�
	CTemplate<float*> t3; // ģ��ƫ�ػ� 1
	CTemplate<decltype(func)> t4; // ģ��ƫ�ػ� 2


	getchar();
	return 0;
}