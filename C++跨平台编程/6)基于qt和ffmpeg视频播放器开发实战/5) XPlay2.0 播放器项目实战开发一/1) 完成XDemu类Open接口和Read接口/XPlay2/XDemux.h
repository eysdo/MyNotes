#pragma once
#include "mutex"
struct AVFormatContext;
struct AVPacket;

class XDemux
{
public:
	// ��ý���ļ�, ������ý�� rtmp http rtsp
	virtual bool Open(const char *url);

	// �ռ���Ҫ�������ͷ�, �ͷ�AVPacket����ռ� �� ���ݿռ� av_packet_free
	virtual AVPacket *Read();

	XDemux();
	virtual ~XDemux();

public:
	int _totalMs = 0; // ��ʱ��(����)

protected:
	std::mutex _mux;
	AVFormatContext *_ic = NULL;  // ���װ������

	// ����Ƶ����. ��ȡʱ��������Ƶ
	int _videoStream = 0;
	int _audioStream = 1;
};

