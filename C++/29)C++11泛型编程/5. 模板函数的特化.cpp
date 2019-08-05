#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

// C++11 模板函数的特化


template <typename T>
void func(T i)
{
	cout << "一般模板: " << i << endl;
}

template <>
void func(int i)
{
	cout << "模板特化: " << i << endl;
}


int main(int argc, char **argv)
{
	int i = 10;
	func(i); //调用特化版本


	getchar();
	return 0;
}