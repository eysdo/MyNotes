#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#pragma comment(lib, "opencv_world320.lib")
using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat src = imread("a01.jpg");
	if (!src.data)
	{
		cout << "open file failed!" << endl;
		getchar();
		return -1;
	}

	namedWindow("原图", CV_WINDOW_NORMAL);
	moveWindow("原图", 100, 100);
	imshow("原图", src);

	Mat dst;
	int d = 3;
	namedWindow("美颜后效果", CV_WINDOW_NORMAL);
	moveWindow("美颜后效果", 600, 100);

	for (;;)
	{
		
		long long b = getTickCount();
		bilateralFilter(src, dst, d, d * 2, d / 2);
		double sec = (double)(getTickCount() - b) / (double)getTickFrequency();
		cout << "sec is " << sec << endl;
		cout << "d = " << d << endl;
		imshow("美颜后效果", dst);
		
		int k = waitKey(0);
		if (k == 'd') d += 2;
		else if (k == 'f') d -= 2;
		else break;
	}
	

	return 0;
}