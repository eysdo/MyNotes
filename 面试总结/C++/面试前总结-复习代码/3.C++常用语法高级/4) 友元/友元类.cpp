#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Girl
{
	// 让Dejan类成为Girl类的友元类
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
		cout << "小姐姐今年 " << girl._age << "岁!!" << endl;
	}
};

int main(int argc, char **argv)
{
	Girl girl(18); // 18岁菇凉

	Dejan d;
	d.getGirlAge(girl);



	getchar();
	return 0;
}