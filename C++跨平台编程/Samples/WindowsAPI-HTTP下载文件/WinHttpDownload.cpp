#include "WinHttpDownload.h"
#include <windows.h>
#include <urlmon.h>

#pragma comment(lib, "urlmon.lib")

class CWinHttpDownload : public WinHttpDownload
{
public:
	/* 判断文件是否存在 */
	bool isFile(wchar_t* path)
	{
		DWORD dwAttribute = GetFileAttributes(path);
		if (dwAttribute == 0XFFFFFFFF)
			return false; //0XFFFFFFFF表示文件不存在
		else
			return true;
	}

	/* 下载文件 */
	bool DownloadFiles(wchar_t* url, wchar_t* downloadPath)
	{
		if (URLDownloadToFile(NULL, url, downloadPath, 0, 0) == S_OK && isFile(downloadPath))
			return true;
		else
			return false;
	}
};

/* 工厂方法 */
WinHttpDownload* WinHttpDownload::Get()
{
	static WinHttpDownload* http_d = NULL;
	if (NULL == http_d)
	{
		static CWinHttpDownload c_http_d;
		http_d = &c_http_d;
	}
	return http_d;
}


WinHttpDownload::WinHttpDownload()
{
}


WinHttpDownload::~WinHttpDownload()
{
}
