#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdexcept>
using namespace cv;
using namespace std;

#pragma comment(lib, "opencv_world320.lib")

int main(int argc, char** argv)
{
	// ���������rtsp url
	char *inUrl = "rtsp://admin:***@192.168.25.10";
	// nginx-rtmp ֱ��������rtmp����URL
	char *outUrl = "rtmp://192.168.163.131/live";
	VideoCapture cam;

	//cam.open(inUrl);
	cam.open(0);
	try {
		if (!cam.isOpened())
		{
			throw exception("cam open failed!");
		}
		cout << "inUrl cam open success!" << endl;
		Mat frame;
		namedWindow("video");
		for (;;)
		{
			// ��ȡrtsp��Ƶ֡, ������Ƶ֡
			if (!cam.grab()) continue;
			// yuvת��Ϊrgb
			if (!cam.retrieve(frame)) continue;
			imshow("vidoe", frame);
			waitKey(1);
		}
	}
	catch (exception &e)
	{
		if (cam.isOpened()) cam.release();
		cout << "error:" << e.what()<<endl;
	}

	getchar();
	return 0;
}