#pragma once
class XAudioPlay
{
public:
	// ����Ƶ����
	virtual bool Open() = 0;
	virtual void Close() = 0;

	// ���ػ����л�û�в��ŵ�ʱ�� (����)
	virtual long long GetNoPlayMs() = 0;

	// ������Ƶ
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

