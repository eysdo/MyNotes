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
//��ȡCPU����
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

		apts = 0;
		av_packet_unref(&apack);
		vpts = 0;
		av_packet_unref(&vpack);
	}

	bool InitAudioCodec()
	{
		if (!(ac = CreateCodec(AV_CODEC_ID_AAC)))
		{
			return false;
		}
		ac->bit_rate = 40000;
		ac->sample_rate = sampleRate;
		ac->sample_fmt = AV_SAMPLE_FMT_FLTP;
		ac->channels = channels;
		ac->channel_layout = av_get_default_channel_layout(channels);
		return OpenCodec(&ac);
	}

	bool InitVideoCodec()
	{
		/// 4 ��ʼ��������������
		// a �ҵ�������
		if (!(vc = CreateCodec(AV_CODEC_ID_H264)))
		{
			return false;
		}

		vc->bit_rate = 50 * 1024 * 8; // ѹ����ÿ����Ƶ��bitλ��С 50kB
		vc->width = outWidth;
		vc->height = outHeight;
		vc->time_base = { 1,fps }; // ʱ����� 1/3��
		vc->framerate = { fps,1 }; // ֡��

		//������Ĵ�С, ����֡һ���ؼ�֡
		vc->gop_size = 50; // ÿ��50һ���ؼ�֡ I֡
		vc->max_b_frames = 0; // û��B֡ I֡����ȫ����P֡
		vc->pix_fmt = AV_PIX_FMT_YUV420P;
		// d �򿪱�����������
		return OpenCodec(&vc);
	}

	AVPacket* EncodeAudio(AVFrame* frame)
	{
		pcm->pts = apts;
		apts += av_rescale_q(pcm->nb_samples, { 1,sampleRate }, ac->time_base);

		int ret = avcodec_send_frame(ac, pcm);
		if (ret != 0) return NULL;

		av_packet_unref(&apack);
		ret = avcodec_receive_packet(ac, &apack);
		if (ret != 0) return NULL;
		return &apack;
	}

	AVPacket* EncodeVideo(AVFrame* frame)
	{
		av_packet_unref(&vpack);
		/// h264����
		frame->pts = vpts;
		vpts++;
		int ret = avcodec_send_frame(vc, frame);
		if (ret != 0) return NULL;
		ret = avcodec_receive_packet(vc, &vpack);
		if (ret != 0 || vpack.size <= 0)
		{
			return NULL;
		}
		return &vpack;
	}

	bool InitScale()
	{
		/// 2 ��ʼ����ʽת��������
		vsc = sws_getCachedContext(vsc,
			inWidth, inHeight,    // Դ����
			AV_PIX_FMT_BGR24,     // Դ���ظ�ʽ
			outWidth, outHeight,  // Ŀ�����
			AV_PIX_FMT_YUV420P,   // Ŀ�����ظ�ʽ
			SWS_BICUBIC, // �ߴ�仯ʹ���㷨
			0, 0, 0
		);
		if (!vsc)
		{
			printf("sws_getCachedContext() failed!");
			return false;
		}

		/// 3 ��ʼ����������ݽṹ
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
			printf(buf);
			return false;
		}
		return true;
	}

	AVFrame* RGBToYUV(char *rgb)
	{
		///rgb to yuv
		// ��������ݽṹ
		uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
		// bgrbgrbgr
		// plane indata[0] bbbb indata[1] gggg indata[2] rrrr
		indata[0] = (uint8_t*)rgb;
		int insize[AV_NUM_DATA_POINTERS] = { 0 };
		// һ��(��)���ݵ��ֽ���
		insize[0] = inWidth * inPixSize;

		int h = sws_scale(
			vsc, indata, insize, 0, inHeight, // Դ����
			yuv->data, yuv->linesize
		);
		if (h <= 0) return NULL;
		return yuv;
	}

	bool InitResample()
	{
		///2 ��Ƶ�ز��� �����ĳ�ʼ��
		asc = swr_alloc_set_opts(asc,
			av_get_default_channel_layout(channels), (AVSampleFormat)outSampleFmt, sampleRate, // �����ʽ
			av_get_default_channel_layout(channels), (AVSampleFormat)inSampleFmt, sampleRate,  // �����ʽ
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
		printf("��Ƶ�ز��� �����ĳ�ʼ���ɹ�!\n");

		///3 ��Ƶ�ز�������ռ����
		pcm = av_frame_alloc();
		pcm->format = outSampleFmt;
		pcm->channels = channels;
		pcm->channel_layout = av_get_default_channel_layout(channels);
		pcm->nb_samples = nbSamples; //һ֡��Ƶһͨ���Ĳ�������
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
			pcm->data, pcm->nb_samples, //�������, ����洢��ַ����������
			indata, pcm->nb_samples);
		if (len <= 0)
		{
			return NULL;
		}
		return pcm;
	}
private:
	bool OpenCodec(AVCodecContext **c)
	{
		//����Ƶ������
		int ret = avcodec_open2(*c, 0, 0);
		if (ret != 0)
		{
			char err[1024] = { 0 };
			av_strerror(ret, err, sizeof(err) - 1);
			printf("%s\n", err);
			avcodec_free_context(c);
			return false;
		}
		printf("avcodec_open2() success!");
		return true;
	}
	AVCodecContext* CreateCodec(AVCodecID cid)
	{
		///4 ��ʼ�������� AV_CODEC_ID_AAC
		AVCodec *codec = avcodec_find_encoder(cid);
		if (!codec)
		{
			printf("avcodec_find_encoder() failed!");
			return NULL;
		}
		//��Ƶ������������
		AVCodecContext* c = avcodec_alloc_context3(codec);
		if (!c)
		{
			printf("avcodec_alloc_context3(codec) failed!");
			return NULL;
		}
		printf("avcodec_alloc_context3(codec) success!");

		c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		c->thread_count = XGetCpuNum();
		return c;
	}

	SwsContext *vsc = NULL;    //���ظ�ʽת��������
	SwrContext *asc = NULL;    //��Ƶ�ز���������
	AVFrame *yuv = NULL;       //�����YUV
	AVFrame *pcm = NULL;       //�ز��������PCM
	AVPacket apack = { 0 };    //��Ƶ֡
	AVPacket vpack = { 0 };    //��Ƶ֡
	int apts = 0;
	int vpts = 0;
};

XMediaEncode* XMediaEncode::Get(unsigned char index)
{
	static bool isFirst = true;
	if (isFirst)
	{
		// ע�����еı������
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
