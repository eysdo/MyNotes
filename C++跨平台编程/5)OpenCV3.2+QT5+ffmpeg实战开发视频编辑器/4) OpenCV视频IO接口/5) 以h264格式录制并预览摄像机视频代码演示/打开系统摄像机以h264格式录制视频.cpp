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
	VideoCapture cam(0);
	if (!cam.isOpened())
	{
		cout << "cam open failed!" << endl;
		getchar();
		return -1;
	}
	cout << "cam open success!" << endl;

	Mat frame;
	VideoWriter vw;
	int fps = cam.get(CAP_PROP_FPS);
	if (fps <= 0) fps = 25;
	int cam_width = cam.get(CAP_PROP_FRAME_WIDTH);
	int cam_height = cam.get(CAP_PROP_FRAME_HEIGHT);
	vw.open("out.avi", 
		VideoWriter::fourcc('X','2','6','4'), 
		fps,
		Size(cam_width, cam_height)
	);
	if (!vw.isOpened())
	{
		cout << "VideoWriter open failed!" << endl;
		getchar();
		return -1;
	}
	cout << "VideoWriter open success!" << endl;

	namedWindow("video");
	for (;;)
	{
		if (!cam.read(frame)) break;
		imshow("video", frame);
		vw.write(frame);
		if (waitKey(5) == 'q') break;
	}
	

	return 0;
}
