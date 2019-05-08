#pragma once

/* Windows Http 文件下载接口 */
class WinHttpDownload
{
public:
	/* 工厂方法 */
	static WinHttpDownload* Get();

	/* 判断文件是否存在 */
	virtual bool isFile(wchar_t* path) = 0;

	/* 下载文件 */
	virtual bool DownloadFiles(wchar_t* url, wchar_t* downloadPath) = 0;


public:
	virtual ~WinHttpDownload();

protected:
	WinHttpDownload();
};

