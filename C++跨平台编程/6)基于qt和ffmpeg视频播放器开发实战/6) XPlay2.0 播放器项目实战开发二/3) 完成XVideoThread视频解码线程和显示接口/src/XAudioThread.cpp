#include "XAudioThread.h"
#include "XDecode.h"
#include "XAudioPlay.h"
#include "XResample.h"
#include <iostream>
using namespace std;

void XAudioThread::Push(AVPacket *pkt)
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

bool XAudioThread::Open(AVCodecParameters *codecpar, int sampleRate, int channels)
{
	if (!codecpar) return false;
	_mux.lock();
	if (!_decode) _decode = new XDecode();
	if (!_resample) _resample = new XResample();
	if (!_a_play) _a_play = XAudioPlay::Get();
	bool re = true;
	if (!_resample->Open(codecpar, false))
	{
		re = false;
		cout << "XResample open failed!" << endl;
	}

	_a_play->_sampleRate = sampleRate;
	_a_play->_channels = channels;
	if (!_a_play->Open())
	{
		re = false;
		cout << "XAudioPlay open failed!" << endl;
	}
	
	if(!_decode->Open(codecpar))
	{
		re = false;
		cout << "audio XDecode open failed!" << endl;
	}
	_mux.unlock();
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void XAudioThread::run()
{
	unsigned char *pcm = new unsigned char[1024 * 1024 * 10];
	while (!_isExit)
	{
		_mux.lock();

		// 没有数据
		if (_packs.empty() || !_decode || !_resample || !_a_play)
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
			// 重采样
			int size = _resample->Resample(frame, pcm);
			// 播放音频
			while (size > 0)
			{
				if (_a_play->GetFree() >= size)
				{
					_a_play->Write(pcm, size);
					break;
				}
				msleep(1);
			}
		}
		_mux.unlock();
	}
	delete pcm;
}


XAudioThread::XAudioThread()
{
}


XAudioThread::~XAudioThread()
{
	// 等待线程退出
	_isExit = true;
	wait();
}
