#pragma once
struct AVCodecParameters;
struct AVCodecContext;
#include <mutex>

class XDecode
{
public:

	// �򿪽�����,���ܳɹ�����ͷ�codecpar�ռ�
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

