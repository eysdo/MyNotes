#include "XVideoThread.h"
#include "XDecode.h"
#include <iostream>
using namespace std;

// 打开,不管成功与否都清理
bool XVideoThread::Open(AVCodecParameters *codecpar, IVideoCall *call, int width, int height)
{
	if (!codecpar) return false;
	_mux.lock();

	// 初始化显示窗口
	_call = call;
	if (_call)
	{
		_call->Init(width, height);
	}

	// 打开解码器
	if (!_decode) _decode = new XDecode();
	bool re = true;
	if (!_decode->Open(codecpar))
	{
		re = false;
		cout << "video XDecode open failed!" << endl;
	}
	_mux.unlock();
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void XVideoThread::Push(AVPacket *pkt)
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

void XVideoThread::run()
{
	while (!_isExit)
	{
		_mux.lock();

		// 没有数据
		if (_packs.empty() || !_decode)
		{
			_mux.unlock();
			msleep(1);
			continue;
		}

		AVPacket *pkt = _packs.front();
		_packs.pop_front();

		bool re = _decode->Send(pkt);
		if (!re)
		{
			_mux.unlock();
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
		_mux.unlock();
	}
}

XVideoThread::XVideoThread()
{
}


XVideoThread::~XVideoThread()
{
	// 等待线程退出
	_isExit = true;
	wait();
}
