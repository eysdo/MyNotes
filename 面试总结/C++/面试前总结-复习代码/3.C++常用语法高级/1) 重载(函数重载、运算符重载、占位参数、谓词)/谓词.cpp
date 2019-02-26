#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

// 谓词是指普通函数或重载的operator()返回值是bool类型的函数对象(仿函数)。
// 如果operator接受一个参数，那么叫做一元谓词,如果接受两个参数，那么叫做二元谓词，谓词可作为一个判断式。


/// 一元谓词 - 仿函数
struct isEmpty
{
	bool operator()(int num)
	{
		return !num;
	}
};
/// 二元谓词 - 仿函数
struct isGreater
{
	bool operator()(int a, int b)
	{
		return a > b;
	}
};

/// 一元谓词 - 普通函数
bool g_isEmpty(int num)
{
	return !num;
}
/// 二元谓词 - 普通函数
bool g_isGreater(int a, int b)
{
	return a > b;
}


template<class T1, class T2>
void test(T1 predicate, T2 num)
{
	if (predicate(num))
		cout << "是空的!" << endl;
	else
		cout << "不是空的!" << endl;
}

template<class T1, class T2>
void test2(T1 a, T1 b, T2 predicate)
{
	if (predicate(a, b))
		cout << a << " 大于 " << b << endl;
	else
		cout << a << " 小于 " << b << endl;
}

int main(int argc, char **argv)
{
	cout << "isEmpty()(0) = " << isEmpty()(0) << endl;
	cout << "isEmpty()(1) = " << isEmpty()(1) << endl;

	// 一元谓词 - 仿函数
	test(isEmpty(), 0); // test(谓词, 0)
	test(isEmpty(), 1);
	// 一元谓词 - 普通函数
	test(g_isEmpty, 0);
	test(g_isEmpty, 1);

	// 二元谓词 - 仿函数
	test2(2, 5, isGreater()); // test2(2, 5, 谓词)
	test2(5, 3, isGreater());
	// 二元谓词 - 普通函数
	test2(2, 5, g_isGreater);
	test2(5, 3, g_isGreater);




	getchar();
	return 0;
}