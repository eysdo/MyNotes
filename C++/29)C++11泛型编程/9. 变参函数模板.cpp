#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tuple>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


// C++11 变参函数模板


// 获取参数个数
template<typename ...Args>
int lenght(Args... args)
{
	return sizeof...(args);
}

// 展开参数包
template<typename F, typename ...Args>
void func(const F& f, Args&&... args)
{
	// 逗号表达式展开参数包
	//int arr[] = {(printarg(args), 0)...};

	// 使用初始化列表 - 进一步改进, 支持lambda表达式
	initializer_list<int>{(f(forward<Args>(args)), 0)...};
}

void test()
{
	func([](auto i) {
		cout << i << endl;
	}, 1, 2, "Hello");
}

void test02()
{
	cout << lenght(1, 2, "hello") << endl;
}

/// 递归函数方式展开参数包
void print() /// 1
{
	cout << endl;
}

template<typename T, typename... Args>
void print(const T& t, const Args&... args) /// 2
{
	cout << t << (sizeof...(args) > 0 ? "," : "");
	print(args...);
}

void test03()
{
	print(1, "hello", "C++", 11);
}

/// 包扩展
template<typename T>
string to_str(const T& r)
{
	stringstream ss;
	ss << "\"" << r << "\"";
	return ss.str();
}

template<typename... Args>
void init_vector(vector<string>& vec, const Args& ...args)
{
	vec.assign({to_str(args)...});
}

void test04()
{
	vector<string> vec;
	init_vector(vec, 1, "Hello", "world");
	cout << "vec.size():" << vec.size() << endl;
	for (auto r : vec)
	{
		cout << r << endl;
	}
}


int main(int argc, char **argv)
{
	test04();



	getchar();
	return 0;
}