#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

// C++11 ��Ա����ģ��
class Printer 
{
public:
	template<typename T>
	void print(const T& t)
	{
		cout << t << endl;
	}
};

int main(int argc, char **argv)
{
	Printer p;
	p.print<const char*>("abc");
	p.print(123);


	getchar();
	return 0;
}