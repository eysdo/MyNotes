#include<iostream>
using namespace std;

int main()
{
#ifdef _WIN64
	cout << "64λ" << endl;
#endif // 64λ
#ifdef WIN32
	cout << "32λ" << endl;
#endif // 32λ
#ifdef _WIN32
	cout << "32λ��64λ" << endl;
#endif // 32λ��64λ win

	getchar();
	return 0;
}