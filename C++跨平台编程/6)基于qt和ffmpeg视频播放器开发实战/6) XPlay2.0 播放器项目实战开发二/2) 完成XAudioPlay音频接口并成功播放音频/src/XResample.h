#pragma once
#include <mutex>
struct AVCodecParameters;
struct SwrContext;
struct AVFrame;

class XResample
{
public:

	// 输出参数和输入参数一致除了采样格式, 输出为S16, 会释放codecpar
	virtual bool Open(AVCodecParameters *codecpar);
	virtual void Close();

	// 返回重采样后大小, 不管成功与否都释放indata空间
	virtual int Resample(AVFrame *indata, unsigned char *data);

	XResample();
	~XResample();

public:
	int outFormat = 1; // AV_SAMPLE_FMT_S16

protected:
	std::mutex _mux;
	SwrContext *_actx = 0;
};

