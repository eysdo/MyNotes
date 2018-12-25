#include <opencv2/highgui.hpp>
#include <string>
#include <iostream>
using namespace cv;
using namespace std;

#pragma comment(lib, "opencv_world320.lib")

int main(int argc, char** argv)
{
	VideoCapture cam;
	namedWindow("video");
	
	string url = "rtsp://admin:***@192.168.25.10:554/onvif1";

	//if (cam.open(url)) // 打开rtsp 摄像头

	if (cam.open(0)) // 打开默认系统的
	{
		cout << "open cam success!" << endl;
	}
	else 
	{
		cout << "openn cam failed!" << endl;
		waitKey(0);
		return - 1;
	}

	Mat frame;
	for (;;)
	{
		cam.read(frame);
		imshow("video", frame);
		waitKey(1);
	}

	getchar();
	return 0;
}