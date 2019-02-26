#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

template<class T1, class T2>
class Dejan
{
public:
	Dejan(T1 Name, T2 Age)
	{
		name = Name;
		age = Age;
	}
	//��Ԫ����������ʵ��
	//��Ԫ�����������࣬������ĳ�Ա����
	friend void printDejan(Dejan<T1, T2> &d)
	{
		cout << "name:" << d.name << " age:" << d.age << endl;
	}
private:
	T1 name;
	T2 age;
};

void test()
{
	Dejan<string, int> d("С���", 18);
	printDejan(d);
}

// 4.����������ŵ�����ģ��������ǰ��
template<class T1, class T2>
class Maker;

// 3.�Ѻ���ģ��������ŵ����ǰ��
template<class T1, class T2>
void printMaker(Maker<T1, T2> &m);

template<class T1, class T2>
class Maker
{
public:
	Maker(T1 Name, T2 Age)
	{
		name = Name;
		age = Age;
	}
	//2.���Ͽղ����б���ǿ�Ƶ��ú���ģ��
	friend void printMaker<>(Maker<T1, T2> &m);
private:
	T1 name;
	T2 age;
};



// 1.����Ԫ����д�ɺ���ģ��
template<class T1, class T2>
void printMaker(Maker<T1, T2> &m)
{
	cout << "name:" << m.name << " age:" << m.age << endl;
}

void test02()
{
	Maker<string, int> m("С���", 20);
	printMaker(m);
}

int main()
{
	test02();

	system("pause");
	return 0;
}