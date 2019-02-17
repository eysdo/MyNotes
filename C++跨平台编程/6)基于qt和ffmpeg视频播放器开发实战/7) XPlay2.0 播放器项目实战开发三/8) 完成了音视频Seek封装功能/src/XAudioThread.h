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
	virtual void Clear();

	void run();

	void SetPause(bool isPause);

	XAudioThread();
	virtual ~XAudioThread();

public:
	long long _pts = 0; // ��ǰ���뵽��pts

	bool _isPause = false;

protected:
	std::mutex _amux;
	XAudioPlay *_a_play = 0;
	XResample *_resample = 0;
};

