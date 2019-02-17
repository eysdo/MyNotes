#pragma once
struct AVCodecParameters;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;
#include <mutex>

extern void XFreePacket(AVPacket **pkt);
extern void XFreeFrame(AVFrame **frame);
class XDecode
{
public:

	// �򿪽�����,���ܳɹ�����ͷ�codecpar�ռ�
	virtual bool Open(AVCodecParameters *codecpar);

	// ���͵������߳�, ���ܳɹ�����ͷ�pkt�ռ�(�����ý������)
	virtual bool Send(AVPacket *pkt);

	// ��ȡ��������, һ��send������Ҫ���Recv
	// ÿ�θ���һ��, �ɵ������ͷ� av_frame_free()
	virtual AVFrame* Recv();

	virtual void Close();
	virtual void Clear();

	XDecode();
	virtual ~XDecode();

public:
	bool _isAudio = false;

	// ��ǰ���뵽��pts
	long long _pts = 0;

protected:
	AVCodecContext *_codec = 0;
	std::mutex _mux;
};

