#include "XAudioThread.h"
#include "XDecode.h"
#include "XAudioPlay.h"
#include "XResample.h"
#include <iostream>
using namespace std;

// 停止线程, 清理资源
void XAudioThread::Close()
{
	XDecodeThread::Close();
	if (_resample)
	{
		_resample->Close();
		_amux.lock();
		delete _resample;
		_resample = NULL;
		_amux.unlock();
	}
	if (_a_play)
	{
		_a_play->Close();
		_amux.lock();
		_a_play = NULL;
		_amux.unlock();
	}
}

bool XAudioThread::Open(AVCodecParameters *codecpar, int sampleRate, int channels)
{
	if (!codecpar) return false;

	Clear();

	_amux.lock();
	_pts = 0;
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
	_amux.unlock();
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void XAudioThread::SetPause(bool isPause)
{
	//_amux.lock();
	_isPause = isPause;
	if (_a_play)
		_a_play->SetPause(_isPause);
	//_amux.unlock();
}

void XAudioThread::run()
{
	unsigned char *pcm = new unsigned char[1024 * 1024 * 10];
	while (!_isExit)
	{
		_amux.lock();
		if (_isPause)
		{
			_amux.unlock();
			msleep(5);
			continue;
		}
		// 没有数据
		/*if (_packs.empty() || !_decode || !_resample || !_a_play)
		{
			_amux.unlock();
			msleep(1);
			continue;
		}*/

		/*AVPacket *pkt = _packs.front();
		_packs.pop_front();*/
		AVPacket *pkt = Pop();

		bool re = _decode->Send(pkt);
		if (!re)
		{
			_amux.unlock();
			msleep(1);
			continue;
		}
		// 一次Send可能有多次recv
		while (!_isExit)
		{
			AVFrame *frame = _decode->Recv();
			if (!frame) break;

			// 减去缓冲中未播放的时间
			_pts = _decode->_pts - _a_play->GetNoPlayMs();

			//cout << "audio pts = " << _pts << endl;

			// 重采样
			int size = _resample->Resample(frame, pcm);
			// 播放音频
			while (!_isExit)
			{
				if (size <= 0) break;
				// 缓冲未播完, 空间不够
				if (_a_play->GetFree() < size || _isPause)
				{
					msleep(1);
					continue;
				}
				_a_play->Write(pcm, size);
				break;
			}
		}
		_amux.unlock();
	}
	delete[] pcm;
}


XAudioThread::XAudioThread()
{
	if (!_resample) _resample = new XResample();
	if (!_a_play) _a_play = XAudioPlay::Get();
}


XAudioThread::~XAudioThread()
{
}
