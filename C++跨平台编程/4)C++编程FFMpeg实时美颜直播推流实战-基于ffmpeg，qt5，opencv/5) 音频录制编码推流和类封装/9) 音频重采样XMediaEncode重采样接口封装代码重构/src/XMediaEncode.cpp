#include "XMediaEncode.h"
extern "C"
{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swresample.lib")

#if defined WIN32 || defined _WIN32
#include <windows.h>
#endif
//获取CPU数量
static int XGetCpuNum()
{
#if defined WIN32 || defined _WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return (int)sysinfo.dwNumberOfProcessors;
#elif defined __linux__
	return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined __APPLE__
	int numCPU = 0;
	int mib[4];
	size_t len = sizeof(numCPU);

	// set the mib for hw.ncpu
	mib[0] = CTL_HW;
	mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

						   // get the number of CPUs from the system
	sysctl(mib, 2, &numCPU, &len, NULL, 0);

	if (numCPU < 1)
	{
		mib[1] = HW_NCPU;
		sysctl(mib, 2, &numCPU, &len, NULL, 0);

		if (numCPU < 1)
			numCPU = 1;
	}
	return (int)numCPU;
#else
	return 1;
#endif
}

class CMediaEncode :public XMediaEncode 
{
public:
	void Close()
	{
		if (vsc)
		{
			sws_freeContext(vsc);
			vsc = NULL;
		}
		if (asc)
		{
			swr_free(&asc);
		}
		if (yuv)
		{
			av_frame_free(&yuv);
		}
		if (vc)
		{
			avcodec_free_context(&vc);
		}

		if (pcm)
		{
			av_frame_free(&pcm);
		}

		vpts = 0;
		av_packet_unref(&pack);
	}

	bool InitVideoCodec()
	{
		/// 4 初始化编码器上下文
		// a 找到编码器
		AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
		if (!codec)
		{
			printf("Can't find h264 encoder!");
			return false;
		}

		// b 创建编码器上下文
		vc = avcodec_alloc_context3(codec);
		if (!vc)
		{
			printf("avcodec_alloc_context3() failed!");
			return false;
		}
		// c 配置编码器参数
		vc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; // 全局参数
		vc->codec_id = codec->id;
		vc->thread_count = XGetCpuNum();

		vc->bit_rate = 50 * 1024 * 8; // 压缩后每秒视频的bit位大小 50kB
		vc->width = outWidth;
		vc->height = outHeight;
		vc->time_base = { 1,fps }; // 时间基数 1/3秒
		vc->framerate = { fps,1 }; // 帧率

		//画面组的大小, 多少帧一个关键帧
		vc->gop_size = 50; // 每到50一个关键帧 I帧
		vc->max_b_frames = 0; // 没有B帧 I帧后面全部是P帧
		vc->pix_fmt = AV_PIX_FMT_YUV420P;
		// d 打开编码器上下文
		int ret = avcodec_open2(vc, 0, 0);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			printf(buf);
			return false;
		}
		printf("avcodec_open2() success!");
		return true;
	}

	AVPacket* EncodeVideo(AVFrame* frame)
	{
		av_packet_unref(&pack);
		/// h264编码
		frame->pts = vpts;
		vpts++;
		int ret = avcodec_send_frame(vc, frame);
		if (ret != 0) return NULL;
		ret = avcodec_receive_packet(vc, &pack);
		if (ret != 0 || pack.size <= 0)
		{
			return NULL;
		}
		return &pack;
	}

	bool InitScale()
	{
		/// 2 初始化格式转换上下文
		vsc = sws_getCachedContext(vsc,
			inWidth, inHeight,    // 源宽、高
			AV_PIX_FMT_BGR24,     // 源像素格式
			outWidth, outHeight,  // 目标宽、高
			AV_PIX_FMT_YUV420P,   // 目标像素格式
			SWS_BICUBIC, // 尺寸变化使用算法
			0, 0, 0
		);
		if (!vsc)
		{
			printf("sws_getCachedContext() failed!");
			return false;
		}

		/// 3 初始化输出的数据结构
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
			printf(buf);
			return false;
		}
		return true;
	}

	AVFrame* RGBToYUV(char *rgb)
	{
		///rgb to yuv
		// 输入的数据结构
		uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
		// bgrbgrbgr
		// plane indata[0] bbbb indata[1] gggg indata[2] rrrr
		indata[0] = (uint8_t*)rgb;
		int insize[AV_NUM_DATA_POINTERS] = { 0 };
		// 一行(宽)数据的字节数
		insize[0] = inWidth * inPixSize;

		int h = sws_scale(
			vsc, indata, insize, 0, inHeight, // 源数据
			yuv->data, yuv->linesize
		);
		if (h <= 0) return NULL;
		return yuv;
	}

	bool InitResample()
	{
		///2 音频重采样 上下文初始化
		asc = swr_alloc_set_opts(asc,
			av_get_default_channel_layout(channels), (AVSampleFormat)outSampleFmt, sampleRate, // 输出格式
			av_get_default_channel_layout(channels), (AVSampleFormat)inSampleFmt, sampleRate,  // 输入格式
			0, 0);
		if (!asc)
		{
			printf("swr_alloc_set_opts() failed!\n");
			return false;
		}
		int ret = swr_init(asc);
		if (ret != 0)
		{
			char err[1024] = { 0 };
			av_strerror(ret, err, sizeof(err) - 1);
			printf("%s\n", err);
			return false;
		}
		printf("音频重采样 上下文初始化成功!\n");

		///3 音频重采样输出空间分配
		pcm = av_frame_alloc();
		pcm->format = outSampleFmt;
		pcm->channels = channels;
		pcm->channel_layout = av_get_default_channel_layout(channels);
		pcm->nb_samples = nbSamples; //一帧音频一通道的采样数量
		ret = av_frame_get_buffer(pcm, 0);
		if (ret != 0)
		{
			char err[1024] = { 0 };
			av_strerror(ret, err, sizeof(err) - 1);
			printf("%s\n", err);
			return false;
		}
		return true;
	}
	AVFrame* Resample(char *data)
	{
		const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
		indata[0] = (uint8_t*)data;
		int len = swr_convert(asc, 
			pcm->data, pcm->nb_samples, //输出参数, 输出存储地址和样本数量
			indata, pcm->nb_samples);
		if (len <= 0)
		{
			return NULL;
		}
		return pcm;
	}
private:
	SwsContext *vsc = NULL;    //像素格式转换上下文
	SwrContext *asc = NULL;    //音频重采样上下文
	AVFrame *yuv = NULL;       //输出的YUV
	AVFrame *pcm = NULL;       //重采样输出的PCM
	AVPacket pack = { 0 };
	int vpts = 0;
};

XMediaEncode* XMediaEncode::Get(unsigned char index)
{
	static bool isFirst = true;
	if (isFirst)
	{
		// 注册所有的编解码器
		avcodec_register_all();
		isFirst = false;
	}
	static CMediaEncode cxm[255];
	return &cxm[index];
}

XMediaEncode::XMediaEncode()
{
}


XMediaEncode::~XMediaEncode()
{
}
