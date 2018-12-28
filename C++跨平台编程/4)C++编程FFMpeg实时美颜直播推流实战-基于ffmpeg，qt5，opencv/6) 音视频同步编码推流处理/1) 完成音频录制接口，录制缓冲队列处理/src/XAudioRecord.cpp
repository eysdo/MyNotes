#include "XAudioRecord.h"
#include <QaudioInput>
#include <QMutex>
#include <list>
class CXAudioRecord :public XAudioRecord
{
public:
	bool isExit = false;
	QMutex mutex;
	std::list<XData> datas;
	int maxList = 100;
	XData Pop()
	{
		mutex.lock();
		if (datas.empty())
		{
			mutex.unlock();
			return XData();
		}
		XData d = datas.front();
		datas.pop_front();
		mutex.unlock();
		return d;
	}
	void run()
	{
		printf("������Ƶ¼���߳�\n");
		//һ�ζ�ȡһ֡��Ƶ���ֽ���
		int readSize = nbSamples * channels * sampleByte;
		

		while (!isExit)
		{
			//��ȡ��¼����Ƶ
			//һ�ζ�ȡһ֡��Ƶ
			if (input->bytesReady() < readSize)
			{
				QThread::msleep(1);
				continue;
			}
			char *buf = new char[readSize];
			int size = 0;
			while (size != readSize)
			{
				int len = io->read(buf + size, readSize - size);
				if (len < 0) break;
				size += len;
			}
			if (size != readSize)
			{
				delete[] buf;
				continue;
			}

			//�Ѿ���ȡһ֡��Ƶ
			XData d;
			d.data = buf;
			d.size = readSize;
			mutex.lock();
			if (datas.size() > maxList)
			{
				datas.front().Drop();
				datas.pop_front();
			}
			datas.push_back(d);
			mutex.unlock();
		}
		printf("�˳���Ƶ¼���߳�\n");
	}

	bool Init() 
	{
		Stop();
		///1 qt��Ƶ��ʼ¼��
		QAudioFormat fmt;
		fmt.setSampleRate(sampleRate);
		fmt.setChannelCount(channels);
		fmt.setSampleSize(sampleByte * 8);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);
		fmt.setSampleType(QAudioFormat::UnSignedInt);
		QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
		if (!info.isFormatSupported(fmt))
		{
			printf("Audio format not support!\n");
			fmt = info.nearestFormat(fmt);
		}
		input = new QAudioInput(fmt);
		//��ʼ¼����Ƶ
		io = input->start();
		if (!io)
			return false;
		QThread::start();
		isExit = false;
		return true;
	}
	void Stop()
	{
		isExit = true;
		wait();
		if (input)
			input->stop();
		if (io)
			io->close();
		input = NULL;
		io = NULL;
	}
private:
	QAudioInput *input = NULL;
	QIODevice *io = NULL;
};

XAudioRecord* XAudioRecord::Get(XAUDIOTYPE type, unsigned char index)
{
	static CXAudioRecord record[255];
	return &record[index];
}

XAudioRecord::XAudioRecord()
{
}


XAudioRecord::~XAudioRecord()
{
}
