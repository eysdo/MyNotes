#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

// C++11 普通函数模板
// template<class T> 等同于 template<typename T>
// <>括号中的参数叫 "模板形参", 模板形参不能为空。

template<typename T>
int compare(const T& left, const T& right)
{
	if (left < right)
		return -1;
	if (right < left)
		return 1;
	return 0;
}

// 函数模板支持默认的形参类型
template<class T=double>
void printValue(T& value)
{
	cout << "val: " << value << " | typeid: " << typeid(value).name() << endl;
}

int main(int argc, char **argv)
{
	int a = 0;
	printValue(a);
	int b = compare<int>(3, 5);
	int c = compare(3, 5);
	cout << "val: " << b << " | typeid: " << typeid(b).name() << endl;
	cout << "val: " << c << " | typeid: " << typeid(b).name() << endl;


	getchar();
	return 0;
}