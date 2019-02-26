#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

// ��ģ��
template<class NameType, class AgeType>
class Dejan
{
public:
	Dejan(NameType name, AgeType age)
	{
		this->name = name;
		this->age = age;
	}

	void printDejan()
	{
		cout << "name:" << this->name << " age:" << this->age << endl;
	}
public:
	NameType name;
	AgeType age;
};

// 1.��ģ��ָ���������������(����)
void func(Dejan<string, int> &p)
{
	p.printDejan();
}

// 2.��ģ���������Ͳ�����
template<class T1, class T2>
void func2(Dejan<T1, T2> &p)
{
	p.printDejan();
}

// 3.������ ģ�滯
template<class T>
void func3(T &p)
{
	p.printDejan();
}

void test()
{
	Dejan<string, int> m("С���", 20);

	func(m);
	func2(m);
	func3(m);
}

int main()
{
	test();

	system("pause");
	return 0;
}