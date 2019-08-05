#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

// C++11 ����ģ������

// ����ģ��֮�䣬����ģ������ͨ����֮��������ء�����������ݵ���ʱ�ṩ�ĺ��������������ܹ�������һ���͵�������İ汾��
// ���������ϣ�һ�㰴������˳���ǣ�
// 1. ��ͨ���� 
// 2. ����ģ�壨�޶���T����ʽ�ģ�ָ�롢���á������ȣ�
// 3. ��ͨģ�壨��Tû���κ����Ƶģ�

template<typename T>
void func(T& t) // ͨ��ģ�庯��
{
	cout << "In generic version template " << t << endl;
}

template<typename T>
void func(T* t) // ָ��汾
{
	cout << "In pointer version template " << *t << endl;
}

void func(string* s) // ��ͨ����
{
	cout << "In normal function " << *s << endl;
}


int main(int argc, char **argv)
{
	int i = 10;

	func(i); // ����ͨ�ð汾���������������޷�ʵ�������߲�ƥ��
	func(&i); // ����ָ��汾��ͨ�ð汾��ȻҲ�����ã����Ǳ�����ѡ��������İ汾
	string s = "abc";
	func(&s); // ������ͨ������ͨ�ð汾������汾��ȻҲ�������ã����Ǳ�����ѡ�����ػ��İ汾
	func<>(&s); // ����ָ��汾��ͨ��<>���߱�����������Ҫ��template��������ͨ����


	getchar();
	return 0;
}