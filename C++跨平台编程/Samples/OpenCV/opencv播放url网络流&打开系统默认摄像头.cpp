#include <iostream>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

#pragma comment(lib, "opencv_world320.lib")

int main(int argc, char **argv)
{
	char *win_name = "video"; // 窗口名
	char *url = "rtmp://live.hkstv.hk.lxdns.com/live/hks1";

	VideoCapture cam;
	namedWindow(win_name);

	//if (!cam.open(url))
	if (!cam.open(0)) // 0  打开默认系统的摄像头
		cout << "open cam failed!" << endl;
	cout << "open cam success!" << endl;

	Mat frame;
	for (;;)
	{
		cam.read(frame);
		imshow(win_name, frame);
		waitKey(1);
	}

	getchar();
	return 0;
}