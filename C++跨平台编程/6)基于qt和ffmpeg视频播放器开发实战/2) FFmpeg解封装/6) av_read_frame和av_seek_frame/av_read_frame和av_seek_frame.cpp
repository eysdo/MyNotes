#include<iostream>
extern "C" {
#include <libavformat/avformat.h>
}
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
using namespace std;
// 这个转换函数是参考opencv来的
static double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

int main(int argc, char** argv)
{
	const char* path = "C:/Users/Dejan/Videos/test.mp4";

	// 初始化封装库
	av_register_all();

	// 初始化网络库 (可以打开rtsp rtmp http 协议的视频)
	avformat_network_init();

	// 参数设置
	AVDictionary *opts = NULL;
	// 设置rtsp流以tcp协议打开
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);
	// 网络延时时间
	av_dict_set(&opts, "max_delay", "500", 0);

	AVFormatContext *ic = NULL;
	int re = avformat_open_input(
		&ic, 
		path, 
		0,    // 0表示自动选择解封装器
		&opts // 参数设置, 比如rtsp的延时时间
	);
	if (re != 0)
	{
		char buf[1024] = {0};
		av_strerror(re, buf, sizeof(buf)-1);
		cout << "open " << path << " failed! :" << buf << endl;
		getchar();
		return -1;
	}
	cout << "open " << path << " success!" << endl;

	// 获取流信息
	re = avformat_find_stream_info(ic, 0);

	// 总时长 毫秒
	int totalMs = ic->duration / (AV_TIME_BASE / 1000);
	cout << "totalMs = " << totalMs << endl;

	// 打印视频流详细信息
	av_dump_format(ic, 0, path, 0);

	// 音视频索引. 读取时区分音视频
	int videoStream = 0;
	int audioStream = 1;
	
	/// 获取音视频流信息方式1:
	// 获取音视频流信息 (遍历, 函数获取)
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVStream *as = ic->streams[i];

		// 音视频共有信息部分
		cout << "format = " << as->codecpar->format << endl;
		cout << "codec_id = " << as->codecpar->codec_id << endl;

		// 音频 AVMEDIA_TYPE_AUDIO
		if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStream = i;
			cout << i << "音频信息" << endl;
			cout << "sample_rate = " << as->codecpar->sample_rate << endl;
			cout << "channels = " << as->codecpar->channels << endl;
			// 一帧数据?? 单通道样本数
			cout << "frame_size = " << as->codecpar->frame_size << endl;
			// 单通道样本数为1024,双通道一帧数据 = 1024 * 2 * 2 = 4096 (单通道样本数*通道数*样本类型占用字节)
			// fps = sample_rate / frame_size (采样率/单通道样本数)

		}
		// 视频 AVMEDIA_TYPE_VIDEO
		else if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			cout << i << "视频信息" << endl;
			cout << "width = " << as->codecpar->width << endl;
			cout << "height = " << as->codecpar->height << endl;
			// 帧率 fps 分数转换
			cout << "video fps = " << r2d(as->avg_frame_rate) << endl;
		}
	}

	/// 获取音视频流信息方式2:
	// 获取视频流
	videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	//AVStream *as = ic->streams[videoStream];

	// malloc AVPacket并初始化
	AVPacket *pkt = av_packet_alloc();
	for (;;)
	{
		int re = av_read_frame(ic, pkt);
		if (re != 0) 
		{
			// 循环播放
			cout << "=========================end========================" << endl;
			int ms = 3000; // 3秒位置 根据时间基数(分数)转换
			long long pos = (double)ms/ (double)1000 * r2d(ic->streams[pkt->stream_index]->time_base);
			av_seek_frame(ic, videoStream, pos, AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME); // AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME  从后面开始查找出关键帧
			continue;
		}

		cout << "pkt->size = " << pkt->size << endl;
		// 显示时间
		cout << "pkt->pts = " << pkt->pts << endl;
		
		// 转换为毫秒, 方便做同步
		cout << "pkt->pts ms = " << pkt->pts * (r2d(ic->streams[pkt->stream_index]->time_base) * 1000) << endl;

		// 解码时间
		cout << "pkt->dts = " << pkt->dts << endl;
		if (pkt->stream_index == videoStream)
		{
			cout << "图像" << endl;
		}
		else if (pkt->stream_index == audioStream)
		{
			cout << "音频" << endl;
		}

		// 释放, 引用计数-1 为0释放空间
		av_packet_unref(pkt);
	}
	av_packet_free(&pkt);


	if (ic)
	{
		// 释放封装上下文, 并且把ic置0
		avformat_close_input(&ic);
	}

	getchar();
	return 0;
}