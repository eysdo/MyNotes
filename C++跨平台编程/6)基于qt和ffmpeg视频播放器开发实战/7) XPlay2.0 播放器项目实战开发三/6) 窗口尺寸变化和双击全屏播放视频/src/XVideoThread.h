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

	XVideoThread();
	virtual ~XVideoThread();


public:
	// ͬ��ʱ��, ���ⲿ����
	long long _syncPts = 0;

protected:
	std::mutex _vmux;
	IVideoCall *_call = 0;
};

