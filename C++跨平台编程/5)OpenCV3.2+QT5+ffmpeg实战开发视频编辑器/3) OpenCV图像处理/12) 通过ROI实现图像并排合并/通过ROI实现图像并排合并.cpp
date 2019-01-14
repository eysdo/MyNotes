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
	Mat img1 = imread("01.png");
	Mat img2 = imread("02.jpg");
	int height = img1.rows;
	int width1 = img1.cols;
	int width2 = img2.cols;
	// 将高图像等比缩放与低图像高度一致
	if (img1.rows > img2.rows)
	{
		height = img2.rows;
		width1 = img1.cols * ((float)img2.rows / (float)img1.rows);
		resize(img1, img1, Size(width1, height));
	}
	else if(img1.rows < img2.rows)
	{
		width2 = img2.cols * ((float)img1.rows / (float)img2.rows);
		resize(img2, img2, Size(width2, height));
	}
	// 创建目标Mat
	Mat dst;
	dst.create(height, width1 + width2, img1.type());
	Mat r1 = dst(Rect(0, 0, width1, height));
	Mat r2 = dst(Rect(width1, 0, width2, height));
	img1.copyTo(r1);
	img2.copyTo(r2);
	
	namedWindow("dst");
	imshow("dst", dst);
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

