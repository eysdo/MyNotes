#include "XVideoThread.h"
#include "XDecode.h"
#include <iostream>
using namespace std;

// 打开,不管成功与否都清理
bool XVideoThread::Open(AVCodecParameters *codecpar, IVideoCall *call, int width, int height)
{
	if (!codecpar) return false;

	Clear();

	_vmux.lock();

	_syncPts = 0;

	// 初始化显示窗口
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

		// 音视频同步
		if (_syncPts > 0 && _syncPts < _decode->_pts)
		{
			_vmux.unlock();
			msleep(1);
			continue;
		}

		AVPacket *pkt = Pop();
		/*
		// 没有数据
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
		// 一次Send可能有多次recv
		while (!_isExit)
		{
			AVFrame *frame = _decode->Recv();
			if (!frame) break;
			// 显示视频
			if (_call)
				_call->Repaint(frame);
		}
		_vmux.unlock();
	}
}

// 解码pts, 如果接收到的解码数据pts >= seekpts return true 并且显示画面
bool XVideoThread::RepaintPts(AVPacket *pkt, long long seekpts)
{
	_vmux.lock();
	bool re = _decode->Send(pkt);
	if (!re) 
	{ 
		_vmux.unlock();
		return true; // 表示结束解码
	}
	AVFrame *frame = _decode->Recv();
	if (!frame)
	{
		_vmux.unlock();
		return false;
	}
	// 到达位置
	if (_decode->_pts >= seekpts)
	{
		if(_call)
			_call->Repaint(frame);
		_vmux.unlock();
		return true;
	}
	XFreeFrame(&frame);
	_vmux.unlock();
	return false;
}

XVideoThread::XVideoThread()
{
}


XVideoThread::~XVideoThread()
{
	
}
