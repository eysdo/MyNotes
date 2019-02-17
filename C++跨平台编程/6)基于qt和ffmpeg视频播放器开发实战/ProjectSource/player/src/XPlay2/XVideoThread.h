#pragma once
#include "XDecodeThread.h"
#include "IVideoCall.h"
struct AVCodecParameters;

/// �������ʾ��Ƶ


class XVideoThread : public XDecodeThread
{
public:
	// ����pts, ������յ��Ľ�������pts >= seekpts return true ������ʾ����
	virtual bool RepaintPts(AVPacket *pkt, long long seekpts);
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

	

protected:
	std::mutex _vmux;
	IVideoCall *_call = 0;
};

