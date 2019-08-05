#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;

// C++11 β�÷�������

template<typename T>
auto sum(T begin, T end) -> decltype(*begin) // -> decltype(*beg) ָ������ֵ����
{
	decltype(*begin) ret = *begin;
	for (T it = begin + 1; it != end; it++)
	{
		ret += *it;
	}
	return ret;
}


int main(int argc, char **argv)
{
	vector<int> vec = {1,2,3};
	auto s = sum(vec.begin(), vec.end());
	cout <<"1+2+3 = " << s <<" | typeid:"<< typeid(s).name() << endl;


	getchar();
	return 0;
}