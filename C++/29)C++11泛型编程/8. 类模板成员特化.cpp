#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


// C++11 类模板成员特化


template<typename T>
class CTemplate
{
public:
	void info();
	static int code;
};


template<typename T>
int CTemplate<T>::code = 10;

template<>
int CTemplate<int>::code = 100;  // 普通静态成员变量的int特化, vs2015报红但并没有错误!


template<typename T>
void CTemplate<T>::info()
{
	cout << "一般模板" << endl;
}

template<>
void CTemplate<int>::info()
{
	cout << "int 特化" << endl;
}


int main(int argc, char **argv)
{
	CTemplate<float> t1;
	t1.info(); // 一般模板
	cout << "Code: " << t1.code << endl;

	CTemplate<int> t2;
	t2.info(); // int 特化
	cout << "Code: " << t2.code << endl;


	getchar();
	return 0;
}