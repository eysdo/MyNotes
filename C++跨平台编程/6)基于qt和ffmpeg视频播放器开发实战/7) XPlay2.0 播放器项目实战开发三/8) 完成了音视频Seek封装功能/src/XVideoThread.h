#pragma once
#include "XDecodeThread.h"
#include "IVideoCall.h"
struct AVCodecParameters;

/// �������ʾ��Ƶ


class XVideoThread : public XDecodeThread
{
public:
	// ��,���ܳɹ��������
	virtual bool Open(AVCodecParameters *codecpar, IVideoCall *call, int width, int height);
	void run();

	void SetPause(bool isPause);

	XVideoThread();
	virtual ~XVideoThread();


public:
	// ͬ��ʱ��, ���ⲿ����
	long long _syncPts = 0;

	bool _isPause = false;

	IVideoCall *_call = 0;

protected:
	std::mutex _vmux;
	
};

