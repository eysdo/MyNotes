#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//�д��麯������г�����
//�����಻��ʵ��������
//ע�⣺�̳��˳����࣬��ô�������е����д��麯����������������д
//��������£���ô����Ҳ���Ϊ������

class Father
{
public:
	//���麯��
	//����ֵ....
	//����������
	virtual void getInfo1() = 0;
	virtual void getInfo2() = 0;
	virtual void getInfo3() = 0;
	virtual void getInfo4() = 0;
};

void test()
{
	//Father father; error
	Father *p;
}

class Son :public Father
{
public:
	// �麯�� virtual �ɼӿɲ��ӣ�
	virtual void getInfo()
	{
		cout << "Son void getInfo()" << endl;
	}
};

// ע�⣺�̳��˳����࣬��ô�������е����д��麯����������������д
void test2()
{
	//Son son; error
}

class Son2 :public Father
{
public:
	virtual void getInfo()
	{
		cout << "Son2 void getInfo()" << endl;
	}
	virtual void getInfo1()
	{

	}
	virtual void getInfo2()
	{

	}
	virtual void getInfo3()
	{

	}
	virtual void getInfo4()
	{

	}
};

void test3()
{
	Son2 son2;
}

int main()
{


	system("pause");
	return 0;
}