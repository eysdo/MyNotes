#include "XRtmp.h"
extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/time.h>
}
#include <string>
#pragma comment(lib, "avformat.lib")
class CXRtmp :public XRtmp 
{
public:
	void Close()
	{
		if (ic)
		{
			avformat_close_input(&ic);
			vs = NULL;
		}
		vc = NULL;
		url = "";
	}
	bool Init(const char *url)
	{
		///5 封装器和视频流配置
		// a 创建输出封装器上下文
		int ret = avformat_alloc_output_context2(&ic, 0, "flv", url);
		this->url = url;
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			printf(buf);
			return false;
		}
		return true;
	}

	bool AddStream(const AVCodecContext* c)
	{
		if (!c) return false;
		
		// b 添加视频流
		AVStream *st = avformat_new_stream(ic, NULL);
		if (!st)
		{
			printf("avformat_new_stream() failed!");
			return false;
		}
		st->codecpar->codec_tag = 0; // 指定编码格式
		// 从编码器复制参数
		avcodec_parameters_from_context(st->codecpar, c);
		av_dump_format(ic, 0, url.c_str(), 1);
		if (c->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			vc = c;
			vs = st;
		}
		else if(c->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			ac = c;
			as = st;
		}
		return true;
	}

	bool SendHead()
	{
		///6 打开rtmp的网络输出IO
		int ret = avio_open(&ic->pb, url.c_str(), AVIO_FLAG_WRITE);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			printf(buf);
			return false;
		}
		// 写入封装头
		ret = avformat_write_header(ic, NULL);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			printf(buf);
			return false;
		}
		return true;
	}

	bool SendFrame(AVPacket *pack)
	{
		if (!pack || pack->size <= 0 || !pack->data) return false;
		AVRational stime;
		AVRational dtime;
		// 判断是音频还是视频
		if (vs && vc && pack->stream_index == vs->index)
		{
			stime = vc->time_base;
			dtime = vs->time_base;
		}
		else if (as && ac && pack->stream_index == as->index)
		{
			stime = ac->time_base;
			dtime = as->time_base;
		}
		else
		{
			return false;
		}

		// 推流
		pack->pts = av_rescale_q(pack->pts, stime, dtime);
		pack->dts = av_rescale_q(pack->dts, stime, dtime);
		pack->duration = av_rescale_q(pack->duration, stime, dtime);
		int ret = av_interleaved_write_frame(ic, pack);
		if (ret == 0)
		{
			printf("#");
		}
		return true;
	}

private:
	AVFormatContext *ic = NULL; // rtmp flv 封装器
	const AVCodecContext *vc = NULL; // 视频编码器
	const AVCodecContext *ac = NULL; // 音频编码器
	AVStream *vs = NULL; // 视频流
	AVStream *as = NULL; // 音频流
	std::string url = "";
};

// 工厂生产方法
XRtmp* XRtmp::Get(unsigned char index)
{
	static CXRtmp cxr[255];
	static bool isFirst = true;
	if (isFirst)
	{
		// 注册所有的封装器
		av_register_all();

		// 注册所有网络协议
		avformat_network_init();
		isFirst = false;
	}
	return &cxr[index];
}

XRtmp::XRtmp()
{
}


XRtmp::~XRtmp()
{
}
