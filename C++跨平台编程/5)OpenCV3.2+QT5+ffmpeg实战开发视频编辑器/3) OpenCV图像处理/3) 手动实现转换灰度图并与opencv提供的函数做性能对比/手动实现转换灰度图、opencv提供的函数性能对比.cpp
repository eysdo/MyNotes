#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320.lib")

void PrintMs(const char *text = "");
void RGBToGray(Mat &src, Mat &des);

int main(int argc, char** argv)
{
	Mat src = imread("01.png");
	src.create(3000, 4000, CV_8UC3);
	Mat gray;
	Mat mygray;

	PrintMs();
	cvtColor(src, gray, COLOR_BGR2GRAY);
	PrintMs("cvtColor1");

	PrintMs();
	cvtColor(src, gray, COLOR_BGR2GRAY);
	PrintMs("cvtColor2"); // 在多次调用情况下, 还是 opencv 提供的函数效率最高

	PrintMs();
	RGBToGray(src, mygray);
	PrintMs("RGBToGray");

	namedWindow("src");
	namedWindow("gray");
	namedWindow("mygray");

	imshow("src", src);
	imshow("gray", gray);
	imshow("mygray", mygray);

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

void RGBToGray(Mat &src, Mat &des)
{
	// GRay = (R*30 + G*59 + B*11 + 50) / 100
	des.create(src.rows, src.cols, CV_8UC1);
	for (int row = 0; row < src.rows; row++)
	{
		for (int col = 0; col < src.cols; col++)
		{
			Vec3b &m = src.at<Vec3b>(row, col);
			int gray = (m[2] * 30 + m[1] * 59 + m[0] * 11 + 50) / 100;
			des.at<uchar>(row, col) = gray;
		}
	}
}