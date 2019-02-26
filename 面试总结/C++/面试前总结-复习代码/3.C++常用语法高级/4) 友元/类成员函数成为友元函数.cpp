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
	// 让Dejan类的成员函数成为Girl的友元函数
	friend void Dejan::getGirlAge(Girl &girl); // Dejan成员函数
public:
	Girl(int age) { _age = age; }
private:
	int _age = 0;
};

void Dejan::getGirlAge(Girl &girl)
{
	cout << "小姐姐今年 " << girl._age << "岁!!" << endl;
}

int main(int argc, char **argv)
{
	Girl girl(18); // 18岁菇凉

	Dejan d;
	d.getGirlAge(girl);



	getchar();
	return 0;
}