#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "XFilter.h"
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320d.lib")

// һ����ƵԴ
static VideoCapture cap1;
static VideoCapture cap2;
static bool isExit = false;

// ��һ����ƵԴ�ļ�
bool XVideoThread::Open(const std::string file)
{
	Seek(0);
	mutex.lock();
	bool re = cap1.open(file);
	mutex.unlock();
	
	if (!re)
	{
		cout << "VideoCapture open failed!" << endl;
		return false;
	}
	cout << "VideoCapture open success!" << endl;
	fps = cap1.get(CAP_PROP_FPS);
	width = cap1.get(CAP_PROP_FRAME_WIDTH);
	height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0) fps = 25;
	src1file = file;
	return true;
}

// �򿪶�����ƵԴ�ļ�
bool XVideoThread::Open2(const std::string file)
{
	Seek(0);
	mutex.lock();
	bool re = cap2.open(file);
	mutex.unlock();
	if (!re)
	{
		cout << "VideoCapture open2 failed!" << endl;
		return false;
	}
	cout << "VideoCapture open2 success!" << endl;
	//fps = cap2.get(CAP_PROP_FPS);
	width2 = cap2.get(CAP_PROP_FRAME_WIDTH);
	height2 = cap2.get(CAP_PROP_FRAME_HEIGHT);
	//if (fps <= 0) fps = 25;
	return true;
}

// ���ص�ǰ���ŵ�λ��
double XVideoThread::GetPos()
{
	double pos = 0;
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return pos;
	}
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	double cur = cap1.get(CAP_PROP_POS_FRAMES);
	if(count > 0.001) pos = cur / count;
	mutex.unlock();
	return pos;
}

//������Ƶ
static VideoWriter vw;

//��ʼ������Ƶ
bool XVideoThread::StartSave(const std::string filename, int width, int height, bool isColor)
{
	cout << "��ʼ����" << endl;
	Seek(0);
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	if (width <= 0)
	{
		width = cap1.get(CAP_PROP_FRAME_WIDTH);
	}
	if (height <= 0)
	{
		height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	}
	vw.open(filename,
		VideoWriter::fourcc('X','2','6','4'),
		this->fps,
		Size(width, height),
		isColor
		);
	if (!vw.isOpened())
	{
		cout << "start save failed!" << endl;
		mutex.unlock();
		return false;
	}

	this->isWrite = true;
	desFile = filename;
	mutex.unlock();
	return true;
}

//ֹͣ������Ƶ, д����Ƶ֡������
void XVideoThread::StopSave()
{
	cout << "ֹͣ����" << endl;
	mutex.lock();
	vw.release();
	isWrite = false;
	mutex.unlock();
}

// ��ת��Ƶ
///@para frame int ֡λ��
bool XVideoThread::Seek(int frame)
{
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);
	if(cap2.isOpened())
		cap2.set(CAP_PROP_POS_FRAMES, frame);
	mutex.unlock();
	return re;
}

bool XVideoThread::Seek(double pos)
{
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos * count;
	return Seek(frame);
}

XVideoThread::XVideoThread()
{
	start();
}


XVideoThread::~XVideoThread()
{
	mutex.lock();
	isExit = true;
	mutex.unlock();
	wait();
}

void XVideoThread::run()
{
	Mat mat1;
	for (;;)
	{
		mutex.lock();
		if (isExit)
		{
			mutex.unlock();
			break;
		}
		// �ж���Ƶ�Ƿ��
		if (!cap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		if (!isPlay)
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		// ��ȡһ֡��Ƶ,���벢��ɫת��
		if (!cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			//��������βλ��, ֹͣ����
			if (isWrite)
			{
				StopSave();
				SaveEnd();
			}
			msleep(5);
			continue;
		}

		Mat mat2 = mark;
		//ͨ��������������Ƶ
		if (cap2.isOpened())
		{
			cap2.read(mat2);
		}
		// ��ʾͼ��1
		if (!isWrite)
		{
			ViewImage1(mat1);
			if(!mat2.empty())
				ViewImage2(mat2);
		}

		Mat des = XFilter::Get()->Filter(mat1, mat2);

		// ��ʾ���ɺ�ͼ��
		if (!isWrite)
			ViewDes(des);

		int s = 0;
		s = 1000 / fps;
		if (isWrite)
		{
			s = 1;
			vw.write(des);
		}

		msleep(s);
		mutex.unlock();
	}
}
