#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "minunz.hpp"
using namespace std;


int main(int argc, char **argv)
{
	const char* zipfilename = "./123.zip";
	unzFile uf = NULL;

#	ifdef USEWIN32IOAPI
	zlib_filefunc64_def ffunc;
	fill_win32_filefunc64A(&ffunc);
	uf = unzOpen2_64(zipfilename, &ffunc);
#	else
	uf = unzOpen64(zipfilename);
#	endif

	if (!uf)
	{
		printf("Cannot open %s!!\n", zipfilename);
		return -1;
	}
	printf("压缩文件:%s打开成功!!\n", zipfilename);

	if (0 != do_extract(uf, 0, 0, NULL))
	{
		unzClose(uf);
		cout << "解压文件失败!!" << endl;
		getchar();
	}
	cout << "解压文件成功!!" << endl;


	unzClose(uf);


	getchar();
	return 0;
}