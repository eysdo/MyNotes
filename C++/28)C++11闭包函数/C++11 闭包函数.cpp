#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <functional>
using namespace std;


// C++11 闭包函数

struct Test
{
	int i;
	void func(const char* func_name, int x, int y)
	{
		cout << func_name << ": " << x << " " << y << endl;
	}
};
void g_func(const char* func_name, int x, int y)
{
	cout << func_name << ": " << x << " " << y << endl;
}

template<class F, class... Args>
auto callback(F&& f, Args&&... args)
{
	return std::bind(std::forward<F>(f), std::forward<Args>(args)...)();
}

// 函数参数绑定
void test()
{
	// 类函数
	Test test;
	function<void(const char*, int, int)> func_1 = bind(&Test::func, &test, placeholders::_1, placeholders::_2, placeholders::_3);
	func_1("func_1", 1, 2);

	// 全局函数
	function<void(const char*, int, int)> func_2 = bind(&g_func, placeholders::_1, placeholders::_2, placeholders::_3);
	func_2("func_2", 1, 2);
}

// 类变量赋值
void test_2()
{
	Test test;
	test.i = 200;
	function<int &()> func = bind(&Test::i, &test);
	func() = 123; // test.i = 123;
	cout << "test.i : " << test.i << endl;
}

// 闭包函数实现
void test_3()
{
	int c = 666;
	bool ret = callback([=](int a = 123, int b = 233) {
		cout << a <<" "<< b << endl;
		cout << "c : " << c << endl;
		return true;
	});

	cout << "ret = "<< ret << endl;
}

int main(int argc, char **argv)
{
	test_3();

	getchar();
	return 0;
}