#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdexcept>
extern "C"
{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
}
using namespace cv;
using namespace std;

#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "opencv_world320.lib")

int main(int argc, char** argv)
{
	// ���������rtsp url
	char *inUrl = "rtsp://admin:***@192.168.25.10";
	// nginx-rtmp ֱ��������rtmp����URL
	char *outUrl = "rtmp://192.168.163.131/live";
	VideoCapture cam;
	Mat frame;
	namedWindow("video");

	// ���ظ�ʽת��������
	SwsContext *vsc = NULL;
	// ��������ݽṹ
	AVFrame *yuv = NULL;


	//cam.open(inUrl);
	cam.open(0);
	try {
		/////////////////////////////////////////////////
		/// 1 ʹ��OpenCV��rtsp���
		if (!cam.isOpened())
		{
			throw exception("cam open failed!");
		}
		cout << "inUrl cam open success!" << endl;
		int inWidth = cam.get(CAP_PROP_FRAME_WIDTH);
		int inHeight = cam.get(CAP_PROP_FRAME_HEIGHT);

		/// 2 ��ʼ����ʽת��������
		vsc = sws_getCachedContext(vsc,
			inWidth, inHeight,  // Դ����
			AV_PIX_FMT_BGR24,   // Դ���ظ�ʽ
			inWidth, inHeight,  // Ŀ�����
			AV_PIX_FMT_YUV420P, // Ŀ�����ظ�ʽ
			SWS_BICUBIC, // �ߴ�仯ʹ���㷨
			0, 0, 0
			);
		if (!vsc) 
		{
			throw exception("sws_getCachedContext() failed!");
		}
		// ��������ݽṹ
		yuv = av_frame_alloc();
		yuv->format = AV_PIX_FMT_YUV420P;
		yuv->width = inWidth;
		yuv->height = inHeight;
		yuv->pts = 0;
		// ����yuv�ռ�
		int ret = av_frame_get_buffer(yuv, 32);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			throw exception(buf);
		}



		for (;;)
		{
			// ��ȡrtsp��Ƶ֡, ������Ƶ֡
			if (!cam.grab()) continue;
			// yuvת��Ϊrgb
			if (!cam.retrieve(frame)) continue;
			imshow("vidoe", frame);
			waitKey(1);
			// rgb to yuv

			// ��������ݽṹ
			uint8_t *indata[AV_NUM_DATA_POINTERS] = {0};
			// bgrbgr
			// plane indata[0] bbbb indata[1] gggg indata[2] rrrr
			indata[0] = frame.data;
			int insize[AV_NUM_DATA_POINTERS] = { 0 };
			// һ��(��)���ݵ��ֽ���
			insize[0] = frame.cols * frame.elemSize();

			int h = sws_scale(
				vsc, indata, insize, 0, frame.rows, // Դ����
				yuv->data, yuv->linesize
			);
			if (h <= 0) continue;
			cout << h << " " << flush;

			// h264����

			// ����

		}
	}
	catch (exception &e)
	{
		if (cam.isOpened()) cam.release();
		if (vsc) 
		{ 
			sws_freeContext(vsc);
			vsc = NULL;
		}
		cout << "error:" << e.what()<<endl;
	}

	getchar();
	return 0;
}