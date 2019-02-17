#pragma once
#include "XDecodeThread.h"
#include "IVideoCall.h"
struct AVCodecParameters;

/// 解码和显示视频


class XVideoThread : public XDecodeThread
{
public:
	// 解码pts, 如果接收到的解码数据pts >= seekpts return true 并且显示画面
	virtual bool RepaintPts(AVPacket *pkt, long long seekpts);
	// 打开,不管成功与否都清理
	virtual bool Open(AVCodecParameters *codecpar, IVideoCall *call, int width, int height);
	void run();

	void SetPause(bool isPause);

	XVideoThread();
	virtual ~XVideoThread();


public:
	// 同步时间, 由外部传入
	long long _syncPts = 0;

	bool _isPause = false;

	

protected:
	std::mutex _vmux;
	IVideoCall *_call = 0;
};

