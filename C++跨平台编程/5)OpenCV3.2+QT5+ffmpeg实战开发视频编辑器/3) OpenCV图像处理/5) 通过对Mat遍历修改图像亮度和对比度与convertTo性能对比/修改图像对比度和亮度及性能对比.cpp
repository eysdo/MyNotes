#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320.lib")

void PrintMs(const char *text = "");
void ChangeGain(Mat &src, Mat &dst, float a, int b);

int main(int argc, char** argv)
{
	// 调整对比度和亮度
	Mat src = imread("01.png");
	Mat dst;

	PrintMs();
	ChangeGain(src, dst, 2.0, 50);
	PrintMs("ChangeGain");


	Mat dst2;
	PrintMs();
	src.convertTo(dst2, -1, 2.0, 50); // 效率最高
	PrintMs("convertTo");


	namedWindow("src");
	namedWindow("dst");
	namedWindow("dst2");
	imshow("src", src);
	imshow("dst", dst);
	imshow("dst2", dst2);
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

/**
 * @param  a  float  对比度  1.0 ~ 3.0
 * @param  b  int    亮度    0 ~ 100
 */
void ChangeGain(Mat &src, Mat &dst, float a, int b)
{
	// g(rows,cols) = a*f(rows,cols) + b
	dst.create(src.rows, src.cols, src.type());
	for (int row = 0; row < src.rows; row++)
	{
		for (int col = 0; col < src.cols; col++)
		{
			for (int i = 0; i < 3; i++)
			{
				dst.at<Vec3b>(row, col)[i] = saturate_cast<uchar>(a * src.at<Vec3b>(row, col)[i] + b);
			}
		}
	}
}