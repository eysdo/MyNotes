#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
// ��ͨ��̳���ģ��
template<class T1>
class Father
{
public:
	T1 a;
};
// �̳�ʱ��������߱����������T1������ʲô��������
class Son :public Father<int>
{
public:

};

void test01()
{
	Son s;
}

// ��ģ��̳���ģ��
template<class T, class T2>
class Son2 :public Father<T2> // ������һ����߱����������T1��ʲô����
{
public:
	T age;
};

void test02()
{
	Son2<int, string> s2;
}

//��ģ��ļ̳���Ҫע�⣺������߱�����������Ĳ�����������������ʲô����
int main()
{


	system("pause");
	return 0;
}