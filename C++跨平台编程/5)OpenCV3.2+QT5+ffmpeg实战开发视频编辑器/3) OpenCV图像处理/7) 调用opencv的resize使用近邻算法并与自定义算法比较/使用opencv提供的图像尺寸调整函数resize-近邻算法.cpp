#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320.lib")

void PrintMs(const char *text = "");
void xresize(Mat &src, Mat &dst, Size);

int main(int argc, char** argv)
{
	Mat src = imread("01.png"); // 512*512  256  1024
	Mat img256, img1024, dst256;
	PrintMs();
	xresize(src, img256, Size(256, 256));
	PrintMs("img256");
	PrintMs();
	xresize(src, img1024, Size(1024, 1024));
	PrintMs("img1024");

	PrintMs();
	resize(src, dst256, Size(1024, 1024), 0, 0, INTER_NEAREST); // 默认是 双线性插值算法
	PrintMs("dst256");


	namedWindow("src");
	namedWindow("img256");
	namedWindow("img1024");
	namedWindow("dst256");
	imshow("src", src);
	imshow("img256", img256);
	imshow("img1024", img1024);
	imshow("dst256", dst256);
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

void xresize(Mat &src, Mat &dst, Size size)
{
	dst.create(size, src.type());
	// 映射的原图坐标
	int sx, sy = 0;
	float fx = (float)src.cols / dst.cols;
	float fy = (float)src.rows / dst.rows;
	for (int x = 0; x < dst.cols; x++)
	{
		sx = fx * x + 0.5;
		for (int y = 0; y < dst.rows; y++)
		{
			sy = fy * y + 0.5;
			dst.at<Vec3b>(y, x) = src.at<Vec3b>(sy, sx);
		}
	}
}