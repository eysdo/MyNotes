#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
// Gzip
#include "Gzip.h"
using namespace std;

int main(int argc, char **argv)
{
	string str = 
		"Opening the event log positions the file pointer for this" \
		"Opening the event log positions the file pointer for this" \
		"Opening the event log positions the file pointer for this" \
		"Opening the event log positions the file pointer for this" \
		"Opening the event log positions the file pointer for this" \
		"Opening the event log positions the file pointer for this";

	
	cout << "ԭ���ݴ�С:" << str.length() << endl;
	str = Gzip::Get()->Compress(str);
	cout << "ѹ�����С:" << str.length() << endl;
	str = Gzip::Get()->Uncompress(str);
	cout << "��ѹ���С:" << str.length() << endl;

	getchar();
	return 0;
}