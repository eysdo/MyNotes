#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//������̬����������
//1.Ҫ�м̳�
//2.Ҫ��д������麯��
//3.Ҫ����ָ��ָ���������

class Father
{
public:
	// �麯��
	virtual void info()
	{
		cout << "Father void info()" << endl;
	}
};

class Son1 :public Father
{
public:
	// �麯�� virtual �ɼӿɲ��ӣ�
	virtual void info()
	{
		cout << "Son1 void info()" << endl;
	}
};

class Son2 :public Father
{
public:
	void info()
	{
		cout << "Son2 void info()" << endl;
	}
};

class Son3 :public Father
{
public:
	void info()
	{
		cout << "Son3 void info()" << endl;
	}
};

// ҵ��
void func(Father *father)
{
	father->info();
}

void test()
{
	Father *father = NULL;

	father = new Son1;
	func(father);

	father = new Son2;
	func(father);

	father = new Son3;
	func(father);
}

int main()
{
	test();

	system("pause");
	return 0;
}