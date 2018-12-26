#include <QtCore/QCoreApplication>
#include <QaudioInput>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//1 qt音频开始录制
	QAudioFormat fmt;
	fmt.setSampleRate(44100);
	fmt.setChannelCount(2);
	fmt.setSampleSize(16);
	fmt.setCodec("audio/pcm");
	fmt.setByteOrder(QAudioFormat::LittleEndian);
	fmt.setSampleType(QAudioFormat::UnSignedInt);
	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if (!info.isFormatSupported(fmt))
	{
		cout << "Audio format not support!" << endl;
		fmt = info.nearestFormat(fmt);
	}
	QAudioInput *input = new QAudioInput(fmt);
	//开始录制音频
	QIODevice *io = input->start();
	char buf[4096] = { 0 };
	for (;;)
	{
		if (input->bytesReady() > 4096)
		{
			cout<<io->read(buf, sizeof(buf))<<" "<<flush;
		}
	}


	return a.exec();
}
