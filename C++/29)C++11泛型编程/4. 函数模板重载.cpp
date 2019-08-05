#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

// C++11 函数模板重载

// 函数模板之间，函数模板与普通函数之间可以重载。编译器会根据调用时提供的函数参数，调用能够处理这一类型的最特殊的版本。
// 在特殊性上，一般按照如下顺序考虑：
// 1. 普通函数 
// 2. 特殊模板（限定了T的形式的，指针、引用、容器等）
// 3. 普通模板（对T没有任何限制的）

template<typename T>
void func(T& t) // 通用模板函数
{
	cout << "In generic version template " << t << endl;
}

template<typename T>
void func(T* t) // 指针版本
{
	cout << "In pointer version template " << *t << endl;
}

void func(string* s) // 普通函数
{
	cout << "In normal function " << *s << endl;
}


int main(int argc, char **argv)
{
	int i = 10;

	func(i); // 调用通用版本，其他函数或者无法实例化或者不匹配
	func(&i); // 调用指针版本，通用版本虽然也可以用，但是编译器选择最特殊的版本
	string s = "abc";
	func(&s); // 调用普通函数，通用版本和特殊版本虽然也都可以用，但是编译器选择最特化的版本
	func<>(&s); // 调用指针版本，通过<>告诉编译器我们需要用template而不是普通函数


	getchar();
	return 0;
}