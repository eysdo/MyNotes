#pragma once
#include <QThread>
#include <mutex>
#include <list>
struct AVCodecParameters;
struct AVPacket;
class XDecode;
class XAudioPlay;
class XResample;

class XAudioThread:public QThread
{
public:
	// 打开,不管成功与否都清理
	virtual bool Open(AVCodecParameters *codecpar, int sampleRate, int channels);
	virtual void Push(AVPacket *pkt);
	void run();

	XAudioThread();
	virtual ~XAudioThread();

public:
	int _maxList = 100; // 最大队列
	bool _isExit = false;

protected:
	std::list<AVPacket *> _packs;
	std::mutex _mux;
	XDecode *_decode = 0;
	XAudioPlay *_a_play = 0;
	XResample *_resample = 0;
};

