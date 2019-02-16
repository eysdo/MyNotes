#pragma once
struct AVCodecParameters;
struct AVCodecContext;
#include <mutex>

class XDecode
{
public:

	// 打开解码器,不管成功与否都释放codecpar空间
	virtual bool Open(AVCodecParameters *codecpar);

	virtual void Close();
	virtual void Clear();

	XDecode();
	virtual ~XDecode();

public:
	bool _isAudio = false;

protected:
	AVCodecContext *_codec = 0;
	std::mutex _mux;
};

