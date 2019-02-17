#include "XAudioThread.h"
#include "XDecode.h"
#include "XAudioPlay.h"
#include "XResample.h"
#include <iostream>
using namespace std;

// ֹͣ�߳�, ������Դ
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
		// û������
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
		// һ��Send�����ж��recv
		while (!_isExit)
		{
			AVFrame *frame = _decode->Recv();
			if (!frame) break;

			// ��ȥ������δ���ŵ�ʱ��
			_pts = _decode->_pts - _a_play->GetNoPlayMs();

			//cout << "audio pts = " << _pts << endl;

			// �ز���
			int size = _resample->Resample(frame, pcm);
			// ������Ƶ
			while (!_isExit)
			{
				if (size <= 0) break;
				// ����δ����, �ռ䲻��
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
