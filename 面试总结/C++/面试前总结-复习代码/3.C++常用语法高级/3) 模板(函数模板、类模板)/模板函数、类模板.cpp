#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

// 函数模板
template<class T>
void Swap(T &a, T &b)
{
	T c = a;
	a = b;
	b = c;
}

// 类模板
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

// 1.类模版指定传入的数据类型(常用)
void func(Dejan<string, int> &p)
{
	p.info();
}

// 2.类模版数据类型参数化
template<class T1, class T2>
void func2(Dejan<T1, T2> &p)
{
	p.info();
}

// 3.整个类 模版化
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
	//Swap<int>(a, b); // <int>参数列表
	cout << "a = " << a << " b = " << b << endl;

	cout << "===================================" << endl;

	Dejan<string, int> d("Dejan", 18); // 类模版不能自动推导数据类型，必须显示告诉编译器具体类型是什么
	d.info();

	func(d);
	func2(d);
	func3(d);



	getchar();
	return 0;
}