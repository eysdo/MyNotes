#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdexcept>
extern "C"
{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
using namespace cv;
using namespace std;

#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "opencv_world320.lib")

int main(int argc, char** argv)
{
	// ���������rtsp url
	char *inUrl = "rtsp://admin:dejan1314520@192.168.25.101:554/onvif1";
	// nginx-rtmp ֱ��������rtmp����URL
	char *outUrl = "rtmp://192.168.163.131/live";
	// ע�����еı������
	avcodec_register_all();

	// ע�����еķ�װ��
	av_register_all();

	// ע����������Э��
	avformat_network_init();

	VideoCapture cam;
	Mat frame;
	namedWindow("video");

	// ���ظ�ʽת��������
	SwsContext *vsc = NULL;
	// ��������ݽṹ
	AVFrame *yuv = NULL;

	// ������������
	AVCodecContext *vc = NULL;

	// rtmp flv ��װ��
	AVFormatContext *ic = NULL;

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
		//int fps = cam.get(CAP_PROP_FPS); // ϵͳ����ͷ�ᱨ��! [libx264 @ 04e57880] The encoder timebase is not set. error:Invalid argument
		int fps = 3; // ÿ�봫��֡��

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
		/// 3 ��������ݽṹ
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

		/// 4 ��ʼ������������
		// a �ҵ�������
		AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
		if (!codec)
		{
			throw exception("Can't find h264 encoder!");
		}
		
		// b ����������������
		vc = avcodec_alloc_context3(codec);
		if (!vc)
		{
			throw exception("avcodec_alloc_context3() failed!");
		}
		// c ���ñ���������
		vc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; // ȫ�ֲ���
		vc->codec_id = codec->id;
		vc->thread_count = 4;

		vc->bit_rate = 50 * 1024 * 8; // ѹ����ÿ����Ƶ��bitλ��С 50kB
		vc->width = inWidth;
		vc->height = inHeight;
		vc->time_base = { 1,fps }; // ʱ����� 1/3��
		vc->framerate = { fps,1 }; // ֡��

		// ������Ĵ�С, ����֡һ���ؼ�֡
		vc->gop_size = 50; // ÿ��50һ���ؼ�֡ I֡
		vc->max_b_frames = 0; // û��B֡ I֡����ȫ����P֡
		vc->pix_fmt = AV_PIX_FMT_YUV420P;

		// d �򿪱�����������
		ret = avcodec_open2(vc, 0, 0);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			throw exception(buf);
		}
		cout << "avcodec_open2() success!" << endl;
		
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
		avcodec_parameters_from_context(vs->codecpar, vc);
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


		AVPacket pack;
		memset(&pack, 0, sizeof(pack));
		int vpts = 0;
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
			uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
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
			//cout << h << " " << flush;

			/// h264����
			yuv->pts = vpts;
			vpts++;
			ret = avcodec_send_frame(vc, yuv);
			if (ret != 0) continue;
			ret = avcodec_receive_packet(vc, &pack);
			if (ret != 0 || pack.size > 0)
			{
				//cout << "*" <<pack.size<< flush;
			}
			else
			{
				continue;
			}
			// ����
			pack.pts = av_rescale_q(pack.pts, vc->time_base, vs->time_base);
			pack.dts = av_rescale_q(pack.dts, vc->time_base, vs->time_base);
			pack.duration = av_rescale_q(pack->duration, vc->time_base, vs->time_base);
			ret = av_interleaved_write_frame(ic, &pack);
			if (ret == 0)
			{
				cout << "#" << flush;
			}

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
		if (vc)
		{
			avio_closep(&ic->pb);
			avcodec_free_context(&vc);
		}
		cout << "error:" << e.what() << endl;
	}

	getchar();
	return 0;
}