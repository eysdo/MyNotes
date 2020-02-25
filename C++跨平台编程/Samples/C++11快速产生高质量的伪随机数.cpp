#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <random>
using namespace std;

int main(int argc, char **argv)
{
	std::random_device rd;  //获取随机数种子
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(0, 9);
	for (size_t i = 0; i < 5; i++)
	{
		cout << dis(gen) << endl;
	}

	getchar();
	return 0;
}