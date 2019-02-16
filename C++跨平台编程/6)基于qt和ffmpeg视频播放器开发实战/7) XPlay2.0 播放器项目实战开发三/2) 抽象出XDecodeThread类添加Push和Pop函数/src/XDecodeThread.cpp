#include "XDecodeThread.h"
#include "XDecode.h"

// 取出一帧数据, 并出栈, 如果没有返回NULL
AVPacket* XDecodeThread::Pop()
{
	_mux.lock();
	if (_packs.empty())
	{
		_mux.unlock();
		return NULL;
	}
	AVPacket *pkt = _packs.front();
	_packs.pop_front();
	_mux.unlock();
	return pkt;
}

void XDecodeThread::Push(AVPacket *pkt)
{
	if (!pkt) return;

	// 阻塞
	while (!_isExit)
	{
		_mux.lock();
		if (_packs.size() < _maxList)
		{
			_packs.push_back(pkt);
			_mux.unlock();
			break;
		}
		_mux.unlock();
		msleep(1);
	}
}

XDecodeThread::XDecodeThread()
{
	// 打开解码器
	if (!_decode) _decode = new XDecode();
}


XDecodeThread::~XDecodeThread()
{
	// 等待线程退出
	_isExit = true;
	wait();
}
