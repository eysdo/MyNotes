#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// C++11 类模板、成员函数模板

template<typename T>
class Printer 
{
public:
	explicit Printer(const T& param) :t(param) {}
	
	//右值引用
	string to_string()
	{
		stringstream ss;
		ss << t;
		return move(ss.str());
	}

	void print() {
		cout << t << endl;
	}

	// 成员函数模板 - 定义
	template<typename U>
	void add_add_print(const U& u);

private:
	T t;
};

// 成员函数模板 - 定义
// 注意这里要有两层template的说明
template<typename T>
template<typename U>
void Printer<T>::add_add_print(const U& u)
{
	cout << t + u << endl;
}

void test01()
{
	//Printer p(1); //error

	Printer<int> p(3); //ok
	string str = p.to_string();
	cout << str << endl; //结果为3
}

void test02()
{
	Printer<int> p(30);
	p.add_add_print(3.25); // 自动推断U为double，打印出33.25
}

int main(int argc, char **argv)
{
	test02();


	getchar();
	return 0;
}