#include "XDecodeThread.h"
#include "XDecode.h"

// ȡ��һ֡����, ����ջ, ���û�з���NULL
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

	// ����
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
	// �򿪽�����
	if (!_decode) _decode = new XDecode();
}


XDecodeThread::~XDecodeThread()
{
	// �ȴ��߳��˳�
	_isExit = true;
	wait();
}
