#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320.lib")

void PrintMs(const char *text = "");

int main(int argc, char** argv)
{
	Mat src = imread("01.png");
	Rect rect(100, 100, 200, 200);
	Mat roi = src(rect);

	namedWindow("src");
	imshow("src", src);
	namedWindow("roi");
	imshow("roi", roi);
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