#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdexcept>
#include "XMediaEncode.h"
#include "XRtmp.h"
#pragma comment(lib, "opencv_world320.lib")
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// ���������rtsp url
	char *inUrl = "rtsp://admin:dejan1314520@192.168.25.101:554/onvif1";
	// nginx-rtmp ֱ��������rtmp����URL
	char *outUrl = "rtmp://192.168.163.131/live";
	
	// �����������ظ�ʽת��
	XMediaEncode *me = XMediaEncode::Get(0);

	// ��װ����������
	XRtmp *xr = XRtmp::Get(0);
	

	VideoCapture cam;
	Mat frame;
	namedWindow("video");



	

	

	//cam.open(inUrl);
	int ret = 0;
	try {
		/////////////////////////////////////////////////
		/// 1 ʹ��OpenCV��rtsp���
		cam.open(0);
		if (!cam.isOpened())
		{
			throw exception("cam open failed!");
		}
		cout << "inUrl cam open success!" << endl;
		int inWidth = cam.get(CAP_PROP_FRAME_WIDTH);
		int inHeight = cam.get(CAP_PROP_FRAME_HEIGHT);
		//int fps = cam.get(CAP_PROP_FPS); // ϵͳ����ͷ�ᱨ��! [libx264 @ 04e57880] The encoder timebase is not set. error:Invalid argument
		int fps = 25; // ÿ�봫��֡��

		/// 2 ��ʼ����ʽת��������
		/// 3 ��ʼ����������ݽṹ
		me->inWidth = inWidth;
		me->inHeight = inHeight;
		me->outWidth = inWidth;
		me->outHeight = inHeight;
		me->InitScale();
		
		///4 ��ʼ��������������
		//a �ҵ�������
		if (!me->InitVideoCodec())
		{
			throw exception("InitVideoCodec() failed!");
		}
		
		///5 �����װ������Ƶ������
		xr->Init(outUrl);

		// �����Ƶ��
		xr->AddStream(me->vc);
		xr->SendHead();

		for (;;)
		{
			// ��ȡrtsp��Ƶ֡, ������Ƶ֡
			if (!cam.grab()) continue;
			// yuvת��Ϊrgb
			if (!cam.retrieve(frame)) continue;
			imshow("vidoe", frame);
			waitKey(1);
			
			/// rgb to yuv
			me->inPixSize = frame.elemSize();
			AVFrame *yuv = me->RGBToYUV((char*)frame.data);
			if (!yuv) continue;

			///h264����
			AVPacket* pack = me->EncodeVideo(yuv);
			if (!pack) continue;

			///����
			xr->SendFrame(pack);

		}
	}
	catch (exception &e)
	{
		if (cam.isOpened()) cam.release();
		/*if (vsc)
		{
			sws_freeContext(vsc);
			vsc = NULL;
		}*/
		/*if (vc)
		{
			avio_closep(&ic->pb);
			avcodec_free_context(&vc);
		}*/
		cout << "error:" << e.what() << endl;
	}

	getchar();
	return 0;
}