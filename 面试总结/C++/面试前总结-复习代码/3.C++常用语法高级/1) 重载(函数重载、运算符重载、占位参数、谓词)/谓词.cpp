#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

// ν����ָ��ͨ���������ص�operator()����ֵ��bool���͵ĺ�������(�º���)��
// ���operator����һ����������ô����һԪν��,�������������������ô������Ԫν�ʣ�ν�ʿ���Ϊһ���ж�ʽ��


/// һԪν�� - �º���
struct isEmpty
{
	bool operator()(int num)
	{
		return !num;
	}
};
/// ��Ԫν�� - �º���
struct isGreater
{
	bool operator()(int a, int b)
	{
		return a > b;
	}
};

/// һԪν�� - ��ͨ����
bool g_isEmpty(int num)
{
	return !num;
}
/// ��Ԫν�� - ��ͨ����
bool g_isGreater(int a, int b)
{
	return a > b;
}


template<class T1, class T2>
void test(T1 predicate, T2 num)
{
	if (predicate(num))
		cout << "�ǿյ�!" << endl;
	else
		cout << "���ǿյ�!" << endl;
}

template<class T1, class T2>
void test2(T1 a, T1 b, T2 predicate)
{
	if (predicate(a, b))
		cout << a << " ���� " << b << endl;
	else
		cout << a << " С�� " << b << endl;
}

int main(int argc, char **argv)
{
	cout << "isEmpty()(0) = " << isEmpty()(0) << endl;
	cout << "isEmpty()(1) = " << isEmpty()(1) << endl;

	// һԪν�� - �º���
	test(isEmpty(), 0); // test(ν��, 0)
	test(isEmpty(), 1);
	// һԪν�� - ��ͨ����
	test(g_isEmpty, 0);
	test(g_isEmpty, 1);

	// ��Ԫν�� - �º���
	test2(2, 5, isGreater()); // test2(2, 5, ν��)
	test2(5, 3, isGreater());
	// ��Ԫν�� - ��ͨ����
	test2(2, 5, g_isGreater);
	test2(5, 3, g_isGreater);




	getchar();
	return 0;
}