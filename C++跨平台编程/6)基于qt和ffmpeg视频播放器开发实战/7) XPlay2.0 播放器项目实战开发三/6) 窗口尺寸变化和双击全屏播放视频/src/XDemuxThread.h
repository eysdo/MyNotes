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
	// �������󲢴�
	virtual bool Open(const char *url, IVideoCall *call);
	
	// ���������߳�
	virtual void Start();

	// �ر��߳�������Դ
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

