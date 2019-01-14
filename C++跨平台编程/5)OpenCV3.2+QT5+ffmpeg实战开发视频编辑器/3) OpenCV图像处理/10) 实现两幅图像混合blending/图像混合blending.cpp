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
	resize(img2, img2, img1.size());
	Mat dst;
	float a = 0.5;
	//addWeighted(img1, a, img2, 1-a, 80, dst); // 80 ÊÇ ÔöÒç
	addWeighted(img1, a, img2, 1 - a, 1, dst);

	namedWindow("blending");
	imshow("blending", dst);
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

