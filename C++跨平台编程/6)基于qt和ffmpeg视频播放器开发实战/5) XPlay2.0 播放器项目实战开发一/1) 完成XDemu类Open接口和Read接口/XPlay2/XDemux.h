#pragma once
#include "mutex"
struct AVFormatContext;
struct AVPacket;

class XDemux
{
public:
	// 打开媒体文件, 或者流媒体 rtmp http rtsp
	virtual bool Open(const char *url);

	// 空间需要调用者释放, 释放AVPacket对象空间 和 数据空间 av_packet_free
	virtual AVPacket *Read();

	XDemux();
	virtual ~XDemux();

public:
	int _totalMs = 0; // 总时长(毫秒)

protected:
	std::mutex _mux;
	AVFormatContext *_ic = NULL;  // 解封装上下文

	// 音视频索引. 读取时区分音视频
	int _videoStream = 0;
	int _audioStream = 1;
};

