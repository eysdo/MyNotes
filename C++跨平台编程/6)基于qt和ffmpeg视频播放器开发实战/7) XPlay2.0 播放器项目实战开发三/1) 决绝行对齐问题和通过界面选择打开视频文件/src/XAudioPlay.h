#pragma once
class XAudioPlay
{
public:
	// 打开音频播放
	virtual bool Open() = 0;
	virtual void Close() = 0;

	// 返回缓冲中还没有播放的时间 (毫秒)
	virtual long long GetNoPlayMs() = 0;

	// 播放音频
	virtual bool Write(const unsigned char *data, int datasize) = 0;
	virtual int GetFree() = 0;

	static XAudioPlay *Get();

	XAudioPlay();
	virtual ~XAudioPlay();

public:
	int _sampleRate = 44100;
	int _sampleSize = 16;
	int _channels = 2;
};

