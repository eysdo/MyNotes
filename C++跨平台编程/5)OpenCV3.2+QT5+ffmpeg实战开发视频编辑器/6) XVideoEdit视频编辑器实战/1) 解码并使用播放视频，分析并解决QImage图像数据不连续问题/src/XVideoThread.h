#pragma once
#include <QThread>
#include <QMutex>
#include <opencv2/core.hpp>
class XVideoThread:public QThread
{
	Q_OBJECT

public:
	static XVideoThread* Get()
	{
		static XVideoThread vt;
		return &vt;
	}
	// 打开一号视频源文件
	bool Open(const std::string file);
	~XVideoThread();

	// 线程入口函数
	void run();

signals:
	void ViewImage1(cv::Mat mat);

protected:
	QMutex mutex;
	XVideoThread();
};

