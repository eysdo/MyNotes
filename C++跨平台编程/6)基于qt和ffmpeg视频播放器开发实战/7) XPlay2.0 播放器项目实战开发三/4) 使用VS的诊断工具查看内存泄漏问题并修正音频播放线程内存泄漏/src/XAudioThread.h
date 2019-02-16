#pragma once
#include "XDecodeThread.h"
struct AVCodecParameters;
class XAudioPlay;
class XResample;

class XAudioThread : public XDecodeThread
{
public:
	// 打开,不管成功与否都清理
	virtual bool Open(AVCodecParameters *codecpar, int sampleRate, int channels);
	
	// 停止线程, 清理资源
	virtual void Close();
	void run();

	XAudioThread();
	virtual ~XAudioThread();

public:
	long long _pts = 0; // 当前解码到的pts

protected:
	std::mutex _amux;
	XAudioPlay *_a_play = 0;
	XResample *_resample = 0;
};

