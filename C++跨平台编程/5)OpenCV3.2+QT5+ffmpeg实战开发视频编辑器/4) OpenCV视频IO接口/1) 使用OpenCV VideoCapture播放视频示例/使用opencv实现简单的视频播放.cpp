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
	for (;;)
	{
		// 1. read()一步到位读取视频
		/*if (!video.read(frame))
		{
			break;
		}*/
		//if (frame.empty()) break; // 判断方法二

		// 2. 分步骤读取
		// 读帧, 解码
		if (!video.grab())
		{
			break;
		}
		// 转换颜色格式
		if (!video.retrieve(frame))
		{
			break;
		}
		if (frame.empty()) break;


		imshow("video", frame);
		waitKey(30);
	}

	getchar();
	return 0;
}
