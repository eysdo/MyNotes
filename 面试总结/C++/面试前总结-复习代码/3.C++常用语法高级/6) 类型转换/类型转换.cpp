#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

// ��̬ת��
// 1)������������
void test01()
{
	char a = 'a';

	// char  -->  double    static_cast�ؼ���<Ҫת����������>��˭Ҫת����
	double d = static_cast<double>(a);
	cout << d << endl;
}

// 2)�Զ�����������
class Father {};
class Son :public Father {};
class Dejan {};
void test02()
{
	// 1.ָ��
	Father *f = NULL;
	Son *s = NULL;
	// ����ת��,����ȫ
	Son *s1 = static_cast<Son*>(f);
	// ����ת��,��ȫ
	Father *f1 = static_cast<Father*>(s);

	// 2.����
	Father F;
	Son S;
	Father &F1 = F;
	Son &S1 = S;
	// ����ת��,����ȫ
	Son& S2 = static_cast<Son&>(F1);
	// ����ת��,��ȫ
	Father& F2 = static_cast<Father&>(S1);
}

// ��̬ת��
void test03()
{
	/*char a = 'a';  error: �����������Ͳ����ö�̬ת��
	double d = dynamic_cast<double>(a);*/

	Father *f = NULL;
	Son *s = NULL;

	// ����ת��,��ȫ
	Father *f1 = dynamic_cast<Father*>(s);

	// ����ת��������ȫ,����
	//Son *s1 = dynamic_cast<Son*>(f); error
}

// ����ת��
void test04()
{
	const int *p = NULL;
	// const  ---> ��const
	int *newp = const_cast<int*>(p);

	int *pp = NULL;
	// ��const  ---> const
	const int *newpp = const_cast<const int*>(pp);
}

// ���½���ת��
void test05()
{
	int a = 10;
	int *p = reinterpret_cast<int*>(a);

	Father *f = NULL;
	Dejan *d = reinterpret_cast<Dejan*>(f);
}

int main()
{
	test01();

	system("pause");
	return 0;
}