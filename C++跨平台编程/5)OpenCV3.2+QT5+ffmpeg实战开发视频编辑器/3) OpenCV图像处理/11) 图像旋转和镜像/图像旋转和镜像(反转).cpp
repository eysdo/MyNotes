#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320.lib")

void PrintMs(const char *text = "");

int main(int argc, char** argv)
{
	Mat img = imread("01.png");
	Mat rot;
	Mat fl;

	cv::rotate(img, rot, ROTATE_90_CLOCKWISE); // 顺时针旋转90度
	cv::flip(img, fl, 0); // 上下镜像(图片上下反转) 0 上下  1 左右  -1 上下左右

	namedWindow("rot");
	namedWindow("fl");
	imshow("rot", rot);
	imshow("fl", fl);
	waitKey(0);

	getchar();
	return 0;
}

void PrintMs(const char *text)
{
	static long long last = 0;
	long long cur = getTickCount();
	if (last == 0)
	{
		last = cur;
		return;
	}
	long long ms = 0;
	ms = ((double)(cur - last) / getTickFrequency()) * 1000;
	if (*text != 0)
	{
		printf("%s: %d ms\n", text, ms);
	}
	last = 0;
}

