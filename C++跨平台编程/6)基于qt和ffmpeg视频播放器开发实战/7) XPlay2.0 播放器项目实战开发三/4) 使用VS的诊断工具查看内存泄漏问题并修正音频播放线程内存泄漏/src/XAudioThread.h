#pragma once
#include "XDecodeThread.h"
struct AVCodecParameters;
class XAudioPlay;
class XResample;

class XAudioThread : public XDecodeThread
{
public:
	// ��,���ܳɹ��������
	virtual bool Open(AVCodecParameters *codecpar, int sampleRate, int channels);
	
	// ֹͣ�߳�, ������Դ
	virtual void Close();
	void run();

	XAudioThread();
	virtual ~XAudioThread();

public:
	long long _pts = 0; // ��ǰ���뵽��pts

protected:
	std::mutex _amux;
	XAudioPlay *_a_play = 0;
	XResample *_resample = 0;
};

