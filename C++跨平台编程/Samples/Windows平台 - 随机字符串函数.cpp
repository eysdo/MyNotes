#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;


/* WindowsÆ½Ì¨ - Ëæ»ú×Ö·û´® */
string str_rand(int size = 8)
{
	string buf;
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand((unsigned)seed.QuadPart);
	const char *chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < size; i++)
	{
		const int key = rand() % 62;
		buf += chars[key];
	}
	return buf;
}

int main(int argc, char **argv)
{
	for (int i = 0; i < 100; i++)
	{
		cout << str_rand() << endl;
	}
	

	getchar();
	return 0;
}