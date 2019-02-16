#pragma once
#include <QThread>
#include <mutex>
#include "IVideoCall.h"
class XDemux;
class XVideoThread;
class XAudioThread;

class XDemuxThread :public QThread
{
public:
	// 创建对象并打开
	virtual bool Open(const char *url, IVideoCall *call);
	
	// 启动所有线程
	virtual void Start();

	// 关闭线程清理资源
	virtual void Close();

	void run();

	XDemuxThread();
	virtual ~XDemuxThread();

public:
	bool _isExit = false;
	long long _pts = 0;
	long long _totalMs = 0;

protected:
	std::mutex _mux;
	XDemux *_demux = 0;
	XVideoThread *_vt = 0;
	XAudioThread *_at = 0;
};

