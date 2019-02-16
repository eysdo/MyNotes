#include "XDemux.h"
#include <iostream>
using namespace std;
extern "C" {
#include <libavformat/avformat.h>
}
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")

// 这个转换函数是参考opencv来的
static double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

bool XDemux::Open(const char *url)
{
	// 参数设置
	AVDictionary *opts = NULL;
	// 设置rtsp流以tcp协议打开
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);
	// 网络延时时间
	av_dict_set(&opts, "max_delay", "500", 0);

	_mux.lock();
	int re = avformat_open_input(
		&_ic,
		url,
		0,    // 0表示自动选择解封装器
		&opts // 参数设置, 比如rtsp的延时时间
	);
	if (re != 0)
	{
		_mux.unlock();
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "open " << url << " failed! :" << buf << endl;
		return false;
	}
	cout << "open " << url << " success!" << endl;

	// 获取流信息
	re = avformat_find_stream_info(_ic, 0);

	// 总时长 毫秒
	_totalMs = _ic->duration / (AV_TIME_BASE / 1000);
	cout << "totalMs = " << _totalMs << endl;

	// 打印视频流详细信息
	av_dump_format(_ic, 0, url, 0);

	// 获取视频流
	_videoStream = av_find_best_stream(_ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	AVStream *vs = _ic->streams[_videoStream];
	cout << "============================="<<_videoStream << "视频信息" << "=============================" << endl;
	cout << "format = " << vs->codecpar->format << endl;
	cout << "codec_id = " << vs->codecpar->codec_id << endl;
	cout << "width = " << vs->codecpar->width << endl;
	cout << "height = " << vs->codecpar->height << endl;
	// 帧率 fps 分数转换
	cout << "video fps = " << r2d(vs->avg_frame_rate) << endl;


	// 获取音频流
	_audioStream = av_find_best_stream(_ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	AVStream *as = _ic->streams[_audioStream];
	cout << "=============================" << _audioStream << "音频信息" << "=============================" << endl;
	cout << "format = " << as->codecpar->format << endl;
	cout << "codec_id = " << as->codecpar->codec_id << endl;
	cout << "sample_rate = " << as->codecpar->sample_rate << endl;
	cout << "channels = " << as->codecpar->channels << endl;
	// 一帧数据?? 单通道样本数
	cout << "frame_size = " << as->codecpar->frame_size << endl;
	// 单通道样本数为1024,双通道一帧数据 = 1024 * 2 * 2 = 4096 (单通道样本数*通道数*样本类型占用字节)
	// fps = sample_rate / frame_size (采样率/单通道样本数)
	_mux.unlock();

	return true;
}

// 空间需要调用者释放, 释放AVPacket对象空间 和 数据空间 av_packet_free
AVPacket* XDemux::Read()
{
	_mux.lock();
	if (!_ic) // 容错
	{
		_mux.unlock();
		return false;
	}
	AVPacket *pkt = av_packet_alloc();
	// 读取一帧, 并分配空间
	int re = av_read_frame(_ic, pkt);
	if (re != 0)
	{
		_mux.unlock();
		av_packet_free(&pkt);
		return 0;
	}
	// pts转换为毫秒
	pkt->pts = pkt->pts * (r2d(_ic->streams[pkt->stream_index]->time_base) * 1000);
	pkt->dts = pkt->dts * (r2d(_ic->streams[pkt->stream_index]->time_base) * 1000);
	_mux.unlock();
	cout << "pkt->pts:" << pkt->pts << " "<<flush;
	return pkt;
}

XDemux::XDemux()
{
	static bool isFirst = true;
	static std::mutex dmux;
	dmux.lock();
	if (isFirst)
	{
		// 初始化封装库
		av_register_all();

		// 初始化网络库 (可以打开rtsp rtmp http 协议的视频)
		avformat_network_init();
	}
	dmux.unlock();

}


XDemux::~XDemux()
{
}
