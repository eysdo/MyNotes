#pragma once

/* Windows Http �ļ����ؽӿ� */
class WinHttpDownload
{
public:
	/* �������� */
	static WinHttpDownload* Get();

	/* �ж��ļ��Ƿ���� */
	virtual bool isFile(wchar_t* path) = 0;

	/* �����ļ� */
	virtual bool DownloadFiles(wchar_t* url, wchar_t* downloadPath) = 0;


public:
	virtual ~WinHttpDownload();

protected:
	WinHttpDownload();
};

