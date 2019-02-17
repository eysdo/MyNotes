#include "XVideoThread.h"
#include "XDecode.h"
#include <iostream>
using namespace std;

// ��,���ܳɹ��������
bool XVideoThread::Open(AVCodecParameters *codecpar, IVideoCall *call, int width, int height)
{
	if (!codecpar) return false;

	Clear();

	_vmux.lock();

	_syncPts = 0;

	// ��ʼ����ʾ����
	_call = call;
	if (_call)
	{
		_call->Init(width, height);
	}
	_vmux.unlock();
	bool re = true;
	if (!_decode->Open(codecpar))
	{
		re = false;
		cout << "video XDecode open failed!" << endl;
	}
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void XVideoThread::SetPause(bool isPause)
{
	_vmux.lock();
	_isPause = isPause;
	_vmux.unlock();
}

void XVideoThread::run()
{
	while (!_isExit)
	{
		_vmux.lock();
		if (_isPause)
		{
			_vmux.unlock();
			msleep(5);
			continue;
		}

		// ����Ƶͬ��
		if (_syncPts > 0 && _syncPts < _decode->_pts)
		{
			_vmux.unlock();
			msleep(1);
			continue;
		}

		AVPacket *pkt = Pop();
		/*
		// û������
		if (_packs.empty() || !_decode)
		{
			_vmux.unlock();
			msleep(1);
			continue;
		}
		

		AVPacket *pkt = _packs.front();
		_packs.pop_front();
		*/

		bool re = _decode->Send(pkt);
		if (!re)
		{
			_vmux.unlock();
			msleep(1);
			continue;
		}
		// һ��Send�����ж��recv
		while (!_isExit)
		{
			AVFrame *frame = _decode->Recv();
			if (!frame) break;
			// ��ʾ��Ƶ
			if (_call)
				_call->Repaint(frame);
		}
		_vmux.unlock();
	}
}

XVideoThread::XVideoThread()
{
}


XVideoThread::~XVideoThread()
{
	
}
