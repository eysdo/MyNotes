#include "XDemuxThread.h"
#include "XDemux.h"
#include "XVideoThread.h"
#include "XAudioThread.h"
#include <iostream>
using namespace std;

void XDemuxThread::run()
{
	while (!_isExit)
	{
		_mux.lock();
		if (!_demux)
		{
			_mux.unlock();
			msleep(5);
			continue;
		}
		AVPacket *pkt = _demux->Read();
		if (!pkt)
		{
			_mux.unlock();
			msleep(5);
			continue;
		}
		// �ж���������Ƶ
		if (_demux->IsAudio(pkt))
		{
			if(_at)
				_at->Push(pkt);
		}
		else // ��Ƶ
		{
			if (_vt)
				_vt->Push(pkt);
		}

		_mux.unlock();
	}
}

bool XDemuxThread::Open(const char *url, IVideoCall *call)
{
	if (url == 0 || url[0] == '\0')
		return false;

	_mux.lock();
	if (!_demux) _demux = new XDemux();
	if (!_vt) _vt = new XVideoThread();
	if (!_at) _at = new XAudioThread();

	// �򿪽��װ
	bool re = _demux->Open(url);
	if (!re)
	{
		_mux.unlock();
		cout << "_demux->Open(url) failed!" << endl;
		return false;
	}
	// ����Ƶ�������ʹ����߳�
	if (!_vt->Open(_demux->CopyVPara(), call, _demux->_width, _demux->_height))
	{
		re = false;
		cout << "_vt->Open failed!" << endl;
	}
	// ����Ƶ�������ʹ����߳�
	if (!_at->Open(_demux->CopyAPara(), _demux->_sampleRate, _demux->_channels))
	{
		re = false;
		cout << "_at->Open failed!" << endl;
	}
	_mux.unlock();
	cout << "XDemuxThread::Open : " << re << endl;
	return re;
}

// ���������߳�
void XDemuxThread::Start()
{
	_mux.lock();
	QThread::start();
	if (_vt) _vt->start();
	if (_at) _at->start();
	_mux.unlock();
}

XDemuxThread::XDemuxThread()
{
}


XDemuxThread::~XDemuxThread()
{
	_isExit = true;
	wait();
}
