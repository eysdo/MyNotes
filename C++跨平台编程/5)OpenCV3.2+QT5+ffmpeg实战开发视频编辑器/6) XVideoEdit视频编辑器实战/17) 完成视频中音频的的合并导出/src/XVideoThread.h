#pragma once
#include <QThread>
#include <QMutex>
#include <opencv2/core.hpp>
class XVideoThread:public QThread
{
	Q_OBJECT

public:
	int fps = 0;
	int width = 0;
	int height = 0;
	int width2 = 0;
	int height2 = 0;
	std::string src1file;
	std::string desFile;

	//����ģʽ ��ȡ����
	static XVideoThread* Get()
	{
		static XVideoThread vt;
		return &vt;
	}
	// ��һ����ƵԴ�ļ�
	bool Open(const std::string file);

	// �򿪶�����ƵԴ�ļ�
	bool Open2(const std::string file);

	void Play() { mutex.lock(); isPlay = true; mutex.unlock(); }
	void Pause() { mutex.lock(); isPlay = false; mutex.unlock(); }
	
	// ���ص�ǰ���ŵ�λ��
	double GetPos();

	// ��ת��Ƶ
	///@para frame int ֡λ��
	bool Seek(int frame);
	bool Seek(double pos);

	//��ʼ������Ƶ
	bool StartSave(const std::string filename, int width = 0, int height = 0, bool isColor = true);

	//ֹͣ������Ƶ, д����Ƶ֡������
	void StopSave();

	//���ˮӡ
	void SetMark(cv::Mat m)
	{
		mutex.lock();
		this->mark = m;
		mutex.unlock();
	}
	
	~XVideoThread();

	// �߳���ں���
	void run();

signals:
	//��ʾԭ��Ƶ1ͼ��
	void ViewImage1(cv::Mat mat);
	void ViewImage2(cv::Mat mat);

	//��ʾ���ɺ�ͼ��
	void ViewDes(cv::Mat mat);
	void SaveEnd();

protected:
	QMutex mutex;
	
	//�Ƿ�ʼд��Ƶ
	bool isWrite = false;
	bool isPlay = false;
	cv::Mat mark;
	XVideoThread();
};

