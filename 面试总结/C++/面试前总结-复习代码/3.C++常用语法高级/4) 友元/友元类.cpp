#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Girl
{
	// ��Dejan���ΪGirl�����Ԫ��
	friend class Dejan;
public:
	Girl(int age) { _age = age; }
private:
	int _age = 0;
};

class Dejan
{
public:
	void getGirlAge(Girl &girl)
	{
		cout << "С������ " << girl._age << "��!!" << endl;
	}
};

int main(int argc, char **argv)
{
	Girl girl(18); // 18�깽��

	Dejan d;
	d.getGirlAge(girl);



	getchar();
	return 0;
}