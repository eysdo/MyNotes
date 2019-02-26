#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

// 友元函数: 友元函数不属于类，不是类的成员函数

class User
{
public:
	User(const char* name)
	{
		_name = name;
	}

private:
	// 友元函数, 类内定义
	friend void info(User &d)
	{
		cout << d._name << endl;
	}

	// 友元函数, 类外定义
	friend void info2(User &d);

	string _name;
};

// 普通函数, 让这个全局的函数成为User类友元函数
void info2(User &d)
{
	cout << d._name << endl;
}

int main(int argc, char **argv)
{
	User a("小哥哥");
	info(a);
	User b("小姐姐");
	info2(b);


	getchar();
	return 0;
}