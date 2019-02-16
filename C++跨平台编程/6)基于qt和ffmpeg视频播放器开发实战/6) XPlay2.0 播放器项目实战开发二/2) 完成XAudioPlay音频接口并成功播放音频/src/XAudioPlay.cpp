#include "XAudioPlay.h"
#include <iostream>
#include <QAudioFormat>
#include <QAudioOutput>
#include <mutex>
#include <QThread>
using namespace std;
class CXAudioPlay : public XAudioPlay
{
public:
	QAudioOutput *_output = NULL;
	QIODevice *_io = NULL;
	mutex _mux;

public:
	void Close()
	{
		_mux.lock();
		if (_io)
		{
			_io->close();
			_io = NULL;
		}
		if (_output)
		{
			_output->stop();
			delete _output;
			_output = 0;
		}
		_mux.unlock();
	}

	bool Open() 
	{
		Close();
		QAudioFormat fmt;
		fmt.setSampleRate(_sampleRate);
		fmt.setSampleSize(_sampleSize);
		fmt.setChannelCount(_channels);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);
		fmt.setSampleType(QAudioFormat::UnSignedInt);
		_mux.lock();
		_output = new QAudioOutput(fmt);
		_io = _output->start(); // 开始播放
		_mux.unlock();
		if (!_io) false;
		return true;
	}

	// 播放音频
	bool XAudioPlay::Write(const unsigned char *data, int datasize)
	{
		if (!data || datasize <= 0) return false;
		_mux.lock();
		if (!_output || !_io)
		{
			_mux.unlock();
			return false;
		}
		int size = _io->write((char*)data, datasize);
		_mux.unlock();
		if (datasize != size)
			return false;
		return true;
	}
	
	int XAudioPlay::GetFree()
	{
		_mux.lock();
		if (!_output)
		{
			_mux.unlock();
			return 0;
		}
		int free = _output->bytesFree();
		_mux.unlock();
		return free;
	}
};

XAudioPlay* XAudioPlay::Get()
{
	static CXAudioPlay play;
	return &play;
}

XAudioPlay::XAudioPlay()
{
}


XAudioPlay::~XAudioPlay()
{
}
