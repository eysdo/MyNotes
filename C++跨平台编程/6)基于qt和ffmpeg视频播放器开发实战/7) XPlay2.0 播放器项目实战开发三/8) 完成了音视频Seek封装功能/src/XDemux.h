#pragma once
#include "mutex"
struct AVFormatContext;
struct AVPacket;
struct AVCodecParameters;

class XDemux
{
public:
	// 打开媒体文件, 或者流媒体 rtmp http rtsp
	virtual bool Open(const char *url);

	// 空间需要调用者释放, 释放AVPacket对象空间 和 数据空间 av_packet_free
	virtual AVPacket *Read();

	virtual bool IsAudio(AVPacket *pkt);

	// 获取视频参数  返回的空间需要清理 avcodec_parameters_free()
	virtual AVCodecParameters *CopyVPara();

	// 获取音频参数  返回的空间需要清理 avcodec_parameters_free()
	virtual AVCodecParameters *CopyAPara();

	// seek 位置 pos 0.0~1.0
	virtual bool Seek(double pos);

	// 清空读取缓存
	virtual void Clear();
	virtual void Close();


	XDemux();
	virtual ~XDemux();

public:
	int _totalMs = 0; // 总时长(毫秒)
	int _width = 0;
	int _height = 0;

	int _sampleRate = 0;
	int _channels = 0;

	// 音视频索引. 读取时区分音视频
	int _videoStream = 0;
	int _audioStream = 1;

protected:
	std::mutex _mux;
	AVFormatContext *_ic = NULL;  // 解封装上下文

	
};

