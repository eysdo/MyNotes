#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdexcept>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "XMediaEncode.h"

#pragma comment(lib, "avformat.lib")
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

	// ע�����еķ�װ��
	av_register_all();

	// ע����������Э��
	avformat_network_init();

	VideoCapture cam;
	Mat frame;
	namedWindow("video");



	

	// rtmp flv ��װ��
	AVFormatContext *ic = NULL;

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
		
		
		///5 ��װ������Ƶ������
		// a ���������װ��������
		ret = avformat_alloc_output_context2(&ic,0,"flv",outUrl);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			throw exception(buf);
		}
		// b �����Ƶ��
		AVStream *vs = avformat_new_stream(ic, NULL);
		if (!vs)
		{
			throw exception("avformat_new_stream() failed!");
		}
		vs->codecpar->codec_tag = 0; // ָ�������ʽ
		// �ӱ��������Ʋ���
		avcodec_parameters_from_context(vs->codecpar, me->vc);
		av_dump_format(ic, 0, outUrl, 1);

		///6 ��rtmp���������IO
		ret = avio_open(&ic->pb, outUrl,AVIO_FLAG_WRITE);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			throw exception(buf);
		}
		// д���װͷ
		ret = avformat_write_header(ic, NULL);
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
			
			/// rgb to yuv
			me->inPixSize = frame.elemSize();
			AVFrame *yuv = me->RGBToYUV((char*)frame.data);
			if (!yuv) continue;

			///h264����
			AVPacket* pack = me->EncodeVideo(yuv);
			if (!pack) continue;

			// ����
			pack->pts = av_rescale_q(pack->pts, me->vc->time_base, vs->time_base);
			pack->dts = av_rescale_q(pack->dts, me->vc->time_base, vs->time_base);
			ret = av_interleaved_write_frame(ic, pack);
			if (ret == 0)
			{
				cout << "#" << flush;
			}

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