#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


class myClass
{
public:
	void test()
	{
		cout << "void test()" << endl;
	}
	// char test(){} // ÖØÔØerror
	int test(int num) 
	{
		cout << "int ";
		return num;
	}

	double test(double num)
	{
		cout << "double ";
		return num;
	}

	char* test(char* str)
	{
		cout << "char* ";
		return str;
	}
};


int main(int argc, char **argv)
{

	// º¯ÊýÖØÔØ

	myClass a;
	a.test();
	cout << "a.test(123) = " << a.test(123) << endl;
	cout << "a.test(12.8) = " << a.test(12.8) << endl;
	cout << "a.test(\"hello world!\") = " << a.test("hello world!") << endl;




	getchar();
	return 0;
}