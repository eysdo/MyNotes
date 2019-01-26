#include<iostream>
using namespace std;

int main()
{
#ifdef _WIN64
	cout << "64位" << endl;
#endif // 64位
#ifdef WIN32
	cout << "32位" << endl;
#endif // 32位
#ifdef _WIN32
	cout << "32位和64位" << endl;
#endif // 32位和64位 win

	getchar();
	return 0;
}