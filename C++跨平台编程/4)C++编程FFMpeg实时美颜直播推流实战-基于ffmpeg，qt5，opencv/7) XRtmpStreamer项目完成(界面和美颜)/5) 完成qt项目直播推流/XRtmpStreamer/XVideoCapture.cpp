#include <opencv2/highgui.hpp>
#include "XVideoCapture.h"
using namespace cv;
#pragma comment(lib, "opencv_world320.lib")
class CXVideoCapture :public XVideoCapture
{
public:
	VideoCapture cam;
	
	void run()
	{
		Mat frame;
		while (!isExit)
		{
			if (!cam.read(frame))
			{
				msleep(1);
				continue;
			}
			if (frame.empty())
			{
				msleep(1);
				continue;
			}
			//确保数据是连续的
			fmutex.lock();
			for (int i = 0; i < filters.size(); i++)
			{
				Mat des;
				filters[i]->Filter(&frame, &des);
				frame = des;
			}
			fmutex.unlock();

			XData d((char*)frame.data, frame.cols * frame.rows * frame.elemSize(), GetCurTime());
			Push(d);
		}
	}

	bool Init(int camIndex)
	{
		cam.open(camIndex);
		if (!cam.isOpened())
		{
			printf("cam open failed!\n");
			return false;
		}
		printf("%d cam open success!\n", camIndex);
		width = cam.get(CAP_PROP_FRAME_WIDTH);
		height = cam.get(CAP_PROP_FRAME_HEIGHT);
		fps = cam.get(CAP_PROP_FPS);
		if (fps == 0) fps = 25;
		return true;
	}
	bool Init(const char* url)
	{
		cam.open(url);
		if (!cam.isOpened())
		{
			printf("cam open failed!\n");
			return false;
		}
		printf("%s cam open success!\n", url);
		width = cam.get(CAP_PROP_FRAME_WIDTH);
		height = cam.get(CAP_PROP_FRAME_HEIGHT);
		fps = cam.get(CAP_PROP_FPS);
		if (fps == 0) fps = 25;
		return true;
	}
	void Stop() 
	{
		XDataThread::Stop();
		if (cam.isOpened())
		{
			cam.release();
		}
	}
};

XVideoCapture* XVideoCapture::Get(unsigned char index)
{
	static CXVideoCapture xc[255];
	return &xc[index];
}

XVideoCapture::XVideoCapture()
{
}


XVideoCapture::~XVideoCapture()
{
}
