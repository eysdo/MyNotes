#include "WinHttpDownload.h"
#include <windows.h>
#include <urlmon.h>

#pragma comment(lib, "urlmon.lib")

class CWinHttpDownload : public WinHttpDownload
{
public:
	/* �ж��ļ��Ƿ���� */
	bool isFile(wchar_t* path)
	{
		DWORD dwAttribute = GetFileAttributes(path);
		if (dwAttribute == 0XFFFFFFFF)
			return false; //0XFFFFFFFF��ʾ�ļ�������
		else
			return true;
	}

	/* �����ļ� */
	bool DownloadFiles(wchar_t* url, wchar_t* downloadPath)
	{
		if (URLDownloadToFile(NULL, url, downloadPath, 0, 0) == S_OK && isFile(downloadPath))
			return true;
		else
			return false;
	}
};

/* �������� */
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
