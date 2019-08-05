#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


// C++11 类模板的特化与偏特化


template<typename T>
class CTemplate
{
public:
	CTemplate()
	{
		cout << "一般模板" << endl;
	}
};

template<>
class CTemplate<int>
{
public:
	CTemplate()
	{
		cout << "模板特化" << endl;
	}
};

template<typename T>
class CTemplate<T*>
{
public:
	CTemplate()
	{
		cout << "模板偏特化 1" << endl;
	}
};

template<typename T, typename U> // 另外一个偏特化
class CTemplate<T(U)>
{
public:
	CTemplate()
	{
		cout << "-------------------------" << endl;
		cout << "模板偏特化 2" << endl;
		cout << typeid(T).name() << endl;
		cout << typeid(U).name() << endl;
		cout << "-------------------------" << endl;
	}
};

int func(int i)
{
	return 2 * i;
}

int main(int argc, char **argv)
{
	CTemplate<float> t1; // 一般模板
	CTemplate<int> t2; // 模板特化
	CTemplate<float*> t3; // 模板偏特化 1
	CTemplate<decltype(func)> t4; // 模板偏特化 2


	getchar();
	return 0;
}