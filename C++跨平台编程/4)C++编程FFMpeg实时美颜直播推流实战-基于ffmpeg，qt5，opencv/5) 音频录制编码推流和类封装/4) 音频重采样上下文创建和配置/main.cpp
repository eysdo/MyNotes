#include <QtCore/QCoreApplication>
#include <QaudioInput>
#include <iostream>
extern "C"
{
#include <libswresample/swresample.h>
}
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "avutil.lib")
using namespace std;
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	int sampleRate = 44100;
	int channels = 2;
	int sampleSize = 2;

	///1 qt��Ƶ��ʼ¼��
	QAudioFormat fmt;
	fmt.setSampleRate(sampleRate);
	fmt.setChannelCount(channels);
	fmt.setSampleSize(sampleSize * 8);
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
	//��ʼ¼����Ƶ
	QIODevice *io = input->start();

	///2 ��Ƶ�ز��� �����ĳ�ʼ��
	SwrContext *asc = NULL;
	asc = swr_alloc_set_opts(asc,
			av_get_default_channel_layout(channels), AV_SAMPLE_FMT_FLTP, sampleRate, // �����ʽ
			av_get_default_channel_layout(channels), AV_SAMPLE_FMT_S16, sampleRate,  // �����ʽ
			0,0);
	if (!asc)
	{
		cout << "swr_alloc_set_opts() failed!";
		getchar();
		return -1;
	}
	int ret = swr_init(asc);
	if (ret != 0)
	{
		char err[1024] = { 0 };
		av_strerror(ret, err, sizeof(err) - 1);
		cout << err << endl;
		getchar();
		return -1;
	}
	cout << "��Ƶ�ز��� �����ĳ�ʼ���ɹ�!" << endl;

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
