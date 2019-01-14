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
	// ��һ����ƵԴ�ļ�
	bool Open(const std::string file);
	~XVideoThread();

	// �߳���ں���
	void run();

signals:
	void ViewImage1(cv::Mat mat);

protected:
	QMutex mutex;
	XVideoThread();
};

