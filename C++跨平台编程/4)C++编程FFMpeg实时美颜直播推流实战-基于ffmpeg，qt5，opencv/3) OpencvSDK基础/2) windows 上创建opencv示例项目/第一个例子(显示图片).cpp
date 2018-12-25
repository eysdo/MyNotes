#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

#pragma comment(lib,"opencv_world320d.lib")

int main(int argc, char** argv)
{
	Mat image = imread("1.jpeg");
	namedWindow("img");
	imshow("img", image);
	waitKey(0);

	return 0;
}