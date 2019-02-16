#pragma once
#include <list>
#include <mutex>
#include <QThread>
#include "IVideoCall.h"
struct AVCodecParameters;
struct AVPacket;
class XDecode;

/// �������ʾ��Ƶ


class XVideoThread : public QThread
{
public:
	// ��,���ܳɹ��������
	virtual bool Open(AVCodecParameters *codecpar, IVideoCall *call, int width, int height);
	virtual void Push(AVPacket *pkt);
	void run();

	XVideoThread();
	virtual ~XVideoThread();


public:
	int _maxList = 100; // ������
	bool _isExit = false;

protected:
	std::list<AVPacket *> _packs;
	std::mutex _mux;
	XDecode *_decode = 0;
	IVideoCall *_call = 0;
};

