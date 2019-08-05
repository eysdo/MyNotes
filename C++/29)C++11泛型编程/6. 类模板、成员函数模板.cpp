#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// C++11 ��ģ�塢��Ա����ģ��

template<typename T>
class Printer 
{
public:
	explicit Printer(const T& param) :t(param) {}
	
	//��ֵ����
	string to_string()
	{
		stringstream ss;
		ss << t;
		return move(ss.str());
	}

	void print() {
		cout << t << endl;
	}

	// ��Ա����ģ�� - ����
	template<typename U>
	void add_add_print(const U& u);

private:
	T t;
};

// ��Ա����ģ�� - ����
// ע������Ҫ������template��˵��
template<typename T>
template<typename U>
void Printer<T>::add_add_print(const U& u)
{
	cout << t + u << endl;
}

void test01()
{
	//Printer p(1); //error

	Printer<int> p(3); //ok
	string str = p.to_string();
	cout << str << endl; //���Ϊ3
}

void test02()
{
	Printer<int> p(30);
	p.add_add_print(3.25); // �Զ��ƶ�UΪdouble����ӡ��33.25
}

int main(int argc, char **argv)
{
	test02();


	getchar();
	return 0;
}