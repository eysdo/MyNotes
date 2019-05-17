#include <iostream>
#include <string>
#include "Gzip.h"
using namespace std;


int main(int argc, char **argv)
{
	char* src = "just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk" \
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk"\
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk"\
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk"\
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk"\
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk"\
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk"\
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk";
	
	uLong str_len = strlen(src);
	char* z_buf = (char*)new char[str_len + 1]();
	cout << "ԭʼ����:" << src << endl;
	cout << "ԭʼ���ݳ���:" << str_len << endl;

	int gzSize = gzip_str(src, str_len, z_buf, str_len + 1);
	if (gzSize <= 0)
	{
		printf("compress error.\n");
		getchar();
		return -1;
	}
	string zstr_buf;
	zstr_buf.assign(z_buf, gzSize);
	cout << "ѹ��������: " << zstr_buf << endl;
	cout << "ѹ�������ݳ���:" << zstr_buf.length() << endl;
	

	char* unz_buf = (char*)new char[str_len + 1]();
	ungzip_str(zstr_buf.c_str(), gzSize, unz_buf, str_len + 1);
	cout << "��ѹ������:" << unz_buf << endl;
	cout << "��ѹ�����ݳ���:" << strlen(unz_buf) << endl;

	delete[] z_buf;
	delete[] unz_buf;


	getchar();
	return 0;
}

