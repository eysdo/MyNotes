#include <iostream>
#include <Windows.h>
#include "WinHttpDownload.h"
using namespace std;

int main(int argc, char* argv[])
{

	wchar_t* url = TEXT("https://dldir1.qq.com/weixin/Windows/WeChatSetup.exe");
	wchar_t* savefile = TEXT("D:/1.exe");
	cout << WinHttpDownload::Get()->DownloadFiles(url, savefile);


	getchar();
	return 0;
}