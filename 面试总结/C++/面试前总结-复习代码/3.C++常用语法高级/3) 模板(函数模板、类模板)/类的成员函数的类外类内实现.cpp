#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

template<class NameType, class AgeType>
class Dejan
{
public:
	Dejan(NameType name, AgeType age)
	{
		this->name = name;
		this->age = age;
	}

	void printDejan();
	/*{
		cout << "name:" << this->name << " age:" << this->age << endl;
	}*/
public:
	NameType name;
	AgeType age;
};

//��ģ��ĳ�Ա��������ʵ��
//Ҫ�Ѻ���д�ɺ���ģ��

template<class NameType, class AgeType>
void Dejan<NameType, AgeType>::printDejan()
{
	cout << "name:" << this->name << " age:" << this->age << endl;
}

int main()
{
	Dejan<string, int> d("С���", 18);
	d.printDejan();

	system("pause");
	return 0;
}