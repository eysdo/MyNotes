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
	Mat gsrc;
	Mat lsrc;

	pyrDown(src, gsrc); // 使用高斯金字塔向下取样, 图像缩小
	pyrUp(src, lsrc);   // 使用拉普拉斯金字塔向上取样, 图片放大

	namedWindow("src");
	namedWindow("gsrc");
	namedWindow("lsrc");
	imshow("src", src);
	imshow("gsrc", gsrc);
	imshow("lsrc", lsrc);
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

