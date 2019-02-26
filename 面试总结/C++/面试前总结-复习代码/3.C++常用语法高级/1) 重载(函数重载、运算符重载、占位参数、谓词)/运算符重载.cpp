#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

#define echo cout   // 用 echo 取代 cout

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

	friend ostream& operator << (ostream &out, A &a) // 不加 friend 会报错
	{
		out << a._num;
		return out;
	}

	// 前置加加, 返回的是引用
	A& operator ++ ()
	{
		++_num;
		return *this; // (*this)._num 等同于 this->_num
	}

	// 后置加加，先返回，后加加  返回的是值
	// 后置加加, 返回的是新对象
	A operator++(int) // 占位参数必须是int
	{
		A temp(*this);
		++_num;
		return temp;
	}

	// 重载"()"操作符, 仿函数实现
	void operator()()
	{
		cout << "无参的仿函数被执行!!" << endl;
	}
	int operator()(int a, int b)
	{
		return a + b;
	}

	// 重载"[]"操作符 , 返回类型一定要是引用f["key"]才可以被赋值
	A& operator[](const char* key)
	{
		memcpy(_keyname, key, strlen(key));
		return (*this);
	}

};

// 重载 std::cin
istream& operator >> (istream &in, A &a)
{
	cout << "请输入整数:";
	in >> a._num;
	return in;
}


int main(int argc, char **argv)
{
	// 运算符重载

	// +  -  << 重载
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
	cout << "您输入的整数是:" << a << endl;

	// 前置加加 和 后置加加
	A c(2), d(6);
	cout << "++c = " << ++c << endl;
	cout << "d++ = " << d++ << endl;
	cout << "d = " << d << endl;

	// ()  []  重载 

	// 重载"()"操作符  -  仿函数
	A e;
	e();
	cout << "e(6, 9) = " << e(8, 9) << endl;

	// 重载"[]"操作符 , 返回类型一定要是引用f["key"]才可以被赋值
	A f;
	f["key"] = e(8, 9);
	cout << f._keyname << " = " << f["key"] << endl;



	getchar();
	getchar();
	return 0;
}