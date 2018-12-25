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
	// 海康相机的rtsp url
	char *inUrl = "rtsp://admin:dejan1314520@192.168.25.101:554/onvif1";
	// nginx-rtmp 直播服务器rtmp推流URL
	char *outUrl = "rtmp://192.168.163.131/live";
	
	// 编码器和像素格式转换
	XMediaEncode *me = XMediaEncode::Get(0);

	// 注册所有的封装器
	av_register_all();

	// 注册所有网络协议
	avformat_network_init();

	VideoCapture cam;
	Mat frame;
	namedWindow("video");



	

	// rtmp flv 封装器
	AVFormatContext *ic = NULL;

	//cam.open(inUrl);
	int ret = 0;
	try {
		/////////////////////////////////////////////////
		/// 1 使用OpenCV打开rtsp相机
		cam.open(0);
		if (!cam.isOpened())
		{
			throw exception("cam open failed!");
		}
		cout << "inUrl cam open success!" << endl;
		int inWidth = cam.get(CAP_PROP_FRAME_WIDTH);
		int inHeight = cam.get(CAP_PROP_FRAME_HEIGHT);
		//int fps = cam.get(CAP_PROP_FPS); // 系统摄像头会报错! [libx264 @ 04e57880] The encoder timebase is not set. error:Invalid argument
		int fps = 25; // 每秒传输帧数

		/// 2 初始化格式转换上下文
		/// 3 初始化输出的数据结构
		me->inWidth = inWidth;
		me->inHeight = inHeight;
		me->outWidth = inWidth;
		me->outHeight = inHeight;
		me->InitScale();
		
		///4 初始化编码器上下文
		//a 找到编码器
		if (!me->InitVideoCodec())
		{
			throw exception("InitVideoCodec() failed!");
		}
		
		
		///5 封装器和视频流配置
		// a 创建输出封装器上下文
		ret = avformat_alloc_output_context2(&ic,0,"flv",outUrl);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			throw exception(buf);
		}
		// b 添加视频流
		AVStream *vs = avformat_new_stream(ic, NULL);
		if (!vs)
		{
			throw exception("avformat_new_stream() failed!");
		}
		vs->codecpar->codec_tag = 0; // 指定编码格式
		// 从编码器复制参数
		avcodec_parameters_from_context(vs->codecpar, me->vc);
		av_dump_format(ic, 0, outUrl, 1);

		///6 打开rtmp的网络输出IO
		ret = avio_open(&ic->pb, outUrl,AVIO_FLAG_WRITE);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			throw exception(buf);
		}
		// 写入封装头
		ret = avformat_write_header(ic, NULL);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			throw exception(buf);
		}

		for (;;)
		{
			// 读取rtsp视频帧, 解码视频帧
			if (!cam.grab()) continue;
			// yuv转换为rgb
			if (!cam.retrieve(frame)) continue;
			imshow("vidoe", frame);
			waitKey(1);
			
			/// rgb to yuv
			me->inPixSize = frame.elemSize();
			AVFrame *yuv = me->RGBToYUV((char*)frame.data);
			if (!yuv) continue;

			///h264编码
			AVPacket* pack = me->EncodeVideo(yuv);
			if (!pack) continue;

			// 推流
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