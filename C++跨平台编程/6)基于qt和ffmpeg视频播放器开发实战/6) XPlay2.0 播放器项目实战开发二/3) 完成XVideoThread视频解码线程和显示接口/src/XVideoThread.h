#pragma once
#include <list>
#include <mutex>
#include <QThread>
#include "IVideoCall.h"
struct AVCodecParameters;
struct AVPacket;
class XDecode;

/// 解码和显示视频


class XVideoThread : public QThread
{
public:
	// 打开,不管成功与否都清理
	virtual bool Open(AVCodecParameters *codecpar, IVideoCall *call, int width, int height);
	virtual void Push(AVPacket *pkt);
	void run();

	XVideoThread();
	virtual ~XVideoThread();


public:
	int _maxList = 100; // 最大队列
	bool _isExit = false;

protected:
	std::list<AVPacket *> _packs;
	std::mutex _mux;
	XDecode *_decode = 0;
	IVideoCall *_call = 0;
};

