#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

// ����ģ��
template<class T>
void Swap(T &a, T &b)
{
	T c = a;
	a = b;
	b = c;
}

// ��ģ��
template<class T1, class T2>
class Dejan
{
public:
	T1 _name;
	T2 _age;

public:
	Dejan(T1 name, T2 age)
	{
		_name = name;
		_age = age;
	}
	void info()
	{
		cout << "name : " << _name << "  age : " << _age << endl;
	}
};

// 1.��ģ��ָ���������������(����)
void func(Dejan<string, int> &p)
{
	p.info();
}

// 2.��ģ���������Ͳ�����
template<class T1, class T2>
void func2(Dejan<T1, T2> &p)
{
	p.info();
}

// 3.������ ģ�滯
template<class T>
void func3(T &p)
{
	p.info();
}

int main(int argc, char **argv)
{
	int a = 10, b = 12;
	cout << "a = " << a << " b = " << b << endl;
	Swap(a, b);
	//Swap<int>(a, b); // <int>�����б�
	cout << "a = " << a << " b = " << b << endl;

	cout << "===================================" << endl;

	Dejan<string, int> d("Dejan", 18); // ��ģ�治���Զ��Ƶ��������ͣ�������ʾ���߱���������������ʲô
	d.info();

	func(d);
	func2(d);
	func3(d);



	getchar();
	return 0;
}