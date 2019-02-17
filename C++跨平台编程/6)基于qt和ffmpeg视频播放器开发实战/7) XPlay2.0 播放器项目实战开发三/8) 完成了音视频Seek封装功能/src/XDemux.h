#pragma once
#include "mutex"
struct AVFormatContext;
struct AVPacket;
struct AVCodecParameters;

class XDemux
{
public:
	// ��ý���ļ�, ������ý�� rtmp http rtsp
	virtual bool Open(const char *url);

	// �ռ���Ҫ�������ͷ�, �ͷ�AVPacket����ռ� �� ���ݿռ� av_packet_free
	virtual AVPacket *Read();

	virtual bool IsAudio(AVPacket *pkt);

	// ��ȡ��Ƶ����  ���صĿռ���Ҫ���� avcodec_parameters_free()
	virtual AVCodecParameters *CopyVPara();

	// ��ȡ��Ƶ����  ���صĿռ���Ҫ���� avcodec_parameters_free()
	virtual AVCodecParameters *CopyAPara();

	// seek λ�� pos 0.0~1.0
	virtual bool Seek(double pos);

	// ��ն�ȡ����
	virtual void Clear();
	virtual void Close();


	XDemux();
	virtual ~XDemux();

public:
	int _totalMs = 0; // ��ʱ��(����)
	int _width = 0;
	int _height = 0;

	int _sampleRate = 0;
	int _channels = 0;

	// ����Ƶ����. ��ȡʱ��������Ƶ
	int _videoStream = 0;
	int _audioStream = 1;

protected:
	std::mutex _mux;
	AVFormatContext *_ic = NULL;  // ���װ������

	
};

