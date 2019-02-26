#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

#define echo cout   // �� echo ȡ�� cout

class A
{
public:
	int _num = 0;
	char _keyname[20] = { 0 };

public:
	A() {}
	A(int val)
	{
		_num = val;
	}

	// a + 1 , a - 1
	int operator +(int val)
	{
		return _num + val;
	}
	int operator -(int val)
	{
		return _num - val;
	}

	// a + b , a - b
	int operator +(A &obj)
	{
		return _num + obj._num;
	}
	int operator -(A &obj)
	{
		return _num - obj._num;
	}

	// obj<<a<<b;
	A& operator << (A& ojb)
	{
		cout << ojb._num;
		return ojb;
	}

	friend ostream& operator << (ostream &out, A &a) // ���� friend �ᱨ��
	{
		out << a._num;
		return out;
	}

	// ǰ�üӼ�, ���ص�������
	A& operator ++ ()
	{
		++_num;
		return *this; // (*this)._num ��ͬ�� this->_num
	}

	// ���üӼӣ��ȷ��أ���Ӽ�  ���ص���ֵ
	// ���üӼ�, ���ص����¶���
	A operator++(int) // ռλ����������int
	{
		A temp(*this);
		++_num;
		return temp;
	}

	// ����"()"������, �º���ʵ��
	void operator()()
	{
		cout << "�޲εķº�����ִ��!!" << endl;
	}
	int operator()(int a, int b)
	{
		return a + b;
	}

	// ����"[]"������ , ��������һ��Ҫ������f["key"]�ſ��Ա���ֵ
	A& operator[](const char* key)
	{
		memcpy(_keyname, key, strlen(key));
		return (*this);
	}

};

// ���� std::cin
istream& operator >> (istream &in, A &a)
{
	cout << "����������:";
	in >> a._num;
	return in;
}


int main(int argc, char **argv)
{
	// ���������

	// +  -  << ����
	A a(2);
	cout << a + 1 << endl;
	cout << a - 1 << endl;

	A b(3);
	cout << a + b << endl;
	cout << a - b << endl;

	A out;
	out << b << a;

	echo << "hello world!!" << endl;


	cin >> a;
	cout << "�������������:" << a << endl;

	// ǰ�üӼ� �� ���üӼ�
	A c(2), d(6);
	cout << "++c = " << ++c << endl;
	cout << "d++ = " << d++ << endl;
	cout << "d = " << d << endl;

	// ()  []  ���� 

	// ����"()"������  -  �º���
	A e;
	e();
	cout << "e(6, 9) = " << e(8, 9) << endl;

	// ����"[]"������ , ��������һ��Ҫ������f["key"]�ſ��Ա���ֵ
	A f;
	f["key"] = e(8, 9);
	cout << f._keyname << " = " << f["key"] << endl;



	getchar();
	getchar();
	return 0;
}