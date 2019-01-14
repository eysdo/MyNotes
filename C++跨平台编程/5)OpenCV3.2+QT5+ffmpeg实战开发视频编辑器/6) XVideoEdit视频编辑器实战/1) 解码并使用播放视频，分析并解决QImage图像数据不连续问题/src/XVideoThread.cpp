#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
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
	return true;
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
		// 显示图像
		ViewImage1(mat1);
		msleep(40);
		mutex.unlock();
	}
}
