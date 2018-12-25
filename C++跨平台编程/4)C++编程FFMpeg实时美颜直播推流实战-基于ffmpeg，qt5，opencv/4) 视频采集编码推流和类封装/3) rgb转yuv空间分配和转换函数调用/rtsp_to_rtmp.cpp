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
	// 海康相机的rtsp url
	char *inUrl = "rtsp://admin:***@192.168.25.10";
	// nginx-rtmp 直播服务器rtmp推流URL
	char *outUrl = "rtmp://192.168.163.131/live";
	VideoCapture cam;
	Mat frame;
	namedWindow("video");

	// 像素格式转换上下文
	SwsContext *vsc = NULL;
	// 输出的数据结构
	AVFrame *yuv = NULL;


	//cam.open(inUrl);
	cam.open(0);
	try {
		/////////////////////////////////////////////////
		/// 1 使用OpenCV打开rtsp相机
		if (!cam.isOpened())
		{
			throw exception("cam open failed!");
		}
		cout << "inUrl cam open success!" << endl;
		int inWidth = cam.get(CAP_PROP_FRAME_WIDTH);
		int inHeight = cam.get(CAP_PROP_FRAME_HEIGHT);

		/// 2 初始化格式转换上下文
		vsc = sws_getCachedContext(vsc,
			inWidth, inHeight,  // 源宽、高
			AV_PIX_FMT_BGR24,   // 源像素格式
			inWidth, inHeight,  // 目标宽、高
			AV_PIX_FMT_YUV420P, // 目标像素格式
			SWS_BICUBIC, // 尺寸变化使用算法
			0, 0, 0
			);
		if (!vsc) 
		{
			throw exception("sws_getCachedContext() failed!");
		}
		// 输出的数据结构
		yuv = av_frame_alloc();
		yuv->format = AV_PIX_FMT_YUV420P;
		yuv->width = inWidth;
		yuv->height = inHeight;
		yuv->pts = 0;
		// 分配yuv空间
		int ret = av_frame_get_buffer(yuv, 32);
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
			// rgb to yuv

			// 输入的数据结构
			uint8_t *indata[AV_NUM_DATA_POINTERS] = {0};
			// bgrbgr
			// plane indata[0] bbbb indata[1] gggg indata[2] rrrr
			indata[0] = frame.data;
			int insize[AV_NUM_DATA_POINTERS] = { 0 };
			// 一行(宽)数据的字节数
			insize[0] = frame.cols * frame.elemSize();

			int h = sws_scale(
				vsc, indata, insize, 0, frame.rows, // 源数据
				yuv->data, yuv->linesize
			);
			if (h <= 0) continue;
			cout << h << " " << flush;

			// h264编码

			// 推流

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