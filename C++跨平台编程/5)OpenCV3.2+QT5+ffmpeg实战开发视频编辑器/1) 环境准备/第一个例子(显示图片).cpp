#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world320.lib")

int main(int argc, char** argv)
{
	Mat img = imread("01.jpeg");
	namedWindow("img");
	imshow("img",img);
	waitKey(0);

	return 0;
}