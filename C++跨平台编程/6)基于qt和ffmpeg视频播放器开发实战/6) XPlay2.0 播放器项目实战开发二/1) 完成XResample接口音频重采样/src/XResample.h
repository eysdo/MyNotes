#pragma once
#include <mutex>
struct AVCodecParameters;
struct SwrContext;
struct AVFrame;

class XResample
{
public:

	// ����������������һ�³��˲�����ʽ, ���ΪS16, ���ͷ�codecpar
	virtual bool Open(AVCodecParameters *codecpar);
	virtual void Close();

	// �����ز������С, ���ܳɹ�����ͷ�indata�ռ�
	virtual int Resample(AVFrame *indata, unsigned char *data);

	XResample();
	~XResample();

public:
	int outFormat = 1; // AV_SAMPLE_FMT_S16

protected:
	std::mutex _mux;
	SwrContext *_actx = 0;
};

