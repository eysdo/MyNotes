#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "XFilter.h"
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320d.lib")

// 一号视频源
static VideoCapture cap1;
static bool isExit = false;

// 打开一号视频源文件
bool XVideoThread::Open(const std::string file)
{
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
	if (fps <= 0) fps = 25;
	return true;
}

// 返回当前播放的位置
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

// 跳转视频
///@para frame int 帧位置
bool XVideoThread::Seek(int frame)
{
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);
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
		// 判断视频是否打开
		if (!cap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		// 读取一帧视频,解码并颜色转换
		if (!cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		// 显示图像1
		ViewImage1(mat1);

		//通过过滤器处理视频
		Mat des = XFilter::Get()->Filter(mat1, Mat());

		// 显示生成后图像
		ViewDes(des);

		//msleep(40);
		int s = 0;
		s = 1000 / fps;
		msleep(s);
		mutex.unlock();
	}
}
