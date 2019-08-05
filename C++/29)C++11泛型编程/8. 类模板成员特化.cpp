#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


// C++11 ��ģ���Ա�ػ�


template<typename T>
class CTemplate
{
public:
	void info();
	static int code;
};


template<typename T>
int CTemplate<T>::code = 10;

template<>
int CTemplate<int>::code = 100;  // ��ͨ��̬��Ա������int�ػ�, vs2015���쵫��û�д���!


template<typename T>
void CTemplate<T>::info()
{
	cout << "һ��ģ��" << endl;
}

template<>
void CTemplate<int>::info()
{
	cout << "int �ػ�" << endl;
}


int main(int argc, char **argv)
{
	CTemplate<float> t1;
	t1.info(); // һ��ģ��
	cout << "Code: " << t1.code << endl;

	CTemplate<int> t2;
	t2.info(); // int �ػ�
	cout << "Code: " << t2.code << endl;


	getchar();
	return 0;
}