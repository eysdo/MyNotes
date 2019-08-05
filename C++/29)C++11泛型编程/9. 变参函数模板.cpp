#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tuple>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


// C++11 ��κ���ģ��


// ��ȡ��������
template<typename ...Args>
int lenght(Args... args)
{
	return sizeof...(args);
}

// չ��������
template<typename F, typename ...Args>
void func(const F& f, Args&&... args)
{
	// ���ű��ʽչ��������
	//int arr[] = {(printarg(args), 0)...};

	// ʹ�ó�ʼ���б� - ��һ���Ľ�, ֧��lambda���ʽ
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

/// �ݹ麯����ʽչ��������
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

/// ����չ
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