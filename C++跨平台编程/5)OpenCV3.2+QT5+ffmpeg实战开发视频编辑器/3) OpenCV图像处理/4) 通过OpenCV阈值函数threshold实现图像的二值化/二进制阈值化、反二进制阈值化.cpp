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
	Mat src = imread("01.png");
	Mat gray;
	Mat bin;
	Mat ibin;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	// 二进制阈值化
	threshold(gray, bin, 100, 255, THRESH_BINARY);

	// 反二进制阈值化
	threshold(gray, ibin, 100, 255, THRESH_BINARY_INV);

	namedWindow("src");
	namedWindow("gray");
	namedWindow("bin");
	namedWindow("ibin");
	imshow("src", src);
	imshow("gray", gray);
	imshow("bin", bin);
	imshow("ibin", ibin);

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

