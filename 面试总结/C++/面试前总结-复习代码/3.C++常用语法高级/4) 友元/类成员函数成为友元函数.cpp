#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Girl;
class Dejan
{
public:
	void getGirlAge(Girl &girl);
};

class Girl
{
	// ��Dejan��ĳ�Ա������ΪGirl����Ԫ����
	friend void Dejan::getGirlAge(Girl &girl); // Dejan��Ա����
public:
	Girl(int age) { _age = age; }
private:
	int _age = 0;
};

void Dejan::getGirlAge(Girl &girl)
{
	cout << "С������ " << girl._age << "��!!" << endl;
}

int main(int argc, char **argv)
{
	Girl girl(18); // 18�깽��

	Dejan d;
	d.getGirlAge(girl);



	getchar();
	return 0;
}