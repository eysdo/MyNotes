#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

// ��Ԫ����: ��Ԫ�����������࣬������ĳ�Ա����

class User
{
public:
	User(const char* name)
	{
		_name = name;
	}

private:
	// ��Ԫ����, ���ڶ���
	friend void info(User &d)
	{
		cout << d._name << endl;
	}

	// ��Ԫ����, ���ⶨ��
	friend void info2(User &d);

	string _name;
};

// ��ͨ����, �����ȫ�ֵĺ�����ΪUser����Ԫ����
void info2(User &d)
{
	cout << d._name << endl;
}

int main(int argc, char **argv)
{
	User a("С���");
	info(a);
	User b("С���");
	info2(b);


	getchar();
	return 0;
}