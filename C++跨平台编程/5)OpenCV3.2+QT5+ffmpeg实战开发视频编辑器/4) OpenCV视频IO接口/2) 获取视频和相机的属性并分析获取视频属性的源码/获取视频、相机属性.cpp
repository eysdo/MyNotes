#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320.lib")

int main(int argc, char** argv)
{
	VideoCapture video;
	video.open("C:/Users/Dejan/Videos/test.mp4");
	if (!video.isOpened())
	{
		cout << "open file failed!" << endl;
		getchar();
		return -1;
	}
	cout << "open video success!" << endl;
	namedWindow("video");
	Mat frame;
	int fps = video.get(CAP_PROP_FPS);
	int s = 30;
	if (fps != 0) s = 1000 / fps;  // 毫秒转秒单位
	cout << "fps is " << fps << endl;
	int fcount = video.get(CAP_PROP_FRAME_COUNT);
	cout << "total frame is " << fcount << endl;
	cout << "total sec is " << fcount / fps << endl;

	//s = s / 2; // 两倍的速度播放
	for (;;)
	{
		// 1. read()一步到位读取视频
		if (!video.read(frame))
		{
			break;
		}


		imshow("video", frame);
		waitKey(s);
	}

	getchar();
	return 0;
}
