#include <QtCore/QCoreApplication>
#include <QaudioInput>
#include <iostream>
extern "C"
{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "avutil.lib")
#include <QThread>
using namespace std;
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	//注册所有的编解码器
	avcodec_register_all();

	int sampleRate = 44100;
	int channels = 2;
	int sampleByte = 2;
	AVSampleFormat inSampleFmt = AV_SAMPLE_FMT_S16;
	AVSampleFormat outSampleFmt = AV_SAMPLE_FMT_FLTP;

	///1 qt音频开始录制
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
		cout << "Audio format not support!" << endl;
		fmt = info.nearestFormat(fmt);
	}
	QAudioInput *input = new QAudioInput(fmt);
	//开始录制音频
	QIODevice *io = input->start();

	///2 音频重采样 上下文初始化
	SwrContext *asc = NULL;
	asc = swr_alloc_set_opts(asc,
			av_get_default_channel_layout(channels), outSampleFmt, sampleRate, // 输出格式
			av_get_default_channel_layout(channels), inSampleFmt, sampleRate,  // 输入格式
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
	cout << "音频重采样 上下文初始化成功!" << endl;

	///3 音频重采样输出空间分配
	AVFrame *pcm = av_frame_alloc();
	pcm->format = outSampleFmt;
	pcm->channels = channels;
	pcm->channel_layout = av_get_default_channel_layout(channels);
	pcm->nb_samples = 1024; //一帧音频一通道的采样数量
	ret = av_frame_get_buffer(pcm, 0);
	if (ret != 0)
	{
		char err[1024] = { 0 };
		av_strerror(ret, err, sizeof(err) - 1);
		cout << err << endl;
		getchar();
		return -1;
	}

	///4 初始化音频编码器
	AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
	if (!codec)
	{
		cout << "avcodec_find_encoder(AV_CODEC_ID_AAC) failed!" << endl;
		getchar();
		return -1;
	}
	//音频编码器上下文
	AVCodecContext *ac = avcodec_alloc_context3(codec);
	if (!ac)
	{
		cout << "avcodec_alloc_context3(codec) failed!" << endl;
		getchar();
		return -1;
	}
	cout << "avcodec_alloc_context3(codec) success!" << endl;

	ac->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	ac->thread_count = 8;
	ac->bit_rate = 40000;
	ac->sample_rate = sampleRate;
	ac->sample_fmt = AV_SAMPLE_FMT_FLTP;
	ac->channels = channels;
	ac->channel_layout = av_get_default_channel_layout(channels);

	//打开音频编码器
	ret = avcodec_open2(ac, 0, 0);
	if (ret != 0)
	{
		char err[1024] = { 0 };
		av_strerror(ret, err, sizeof(err) - 1);
		cout << err << endl;
		getchar();
		return -1;
	}
	cout << "avcodec_open2() success!" << endl;


	//一次读取一帧音频的字节数
	int readSize = pcm->nb_samples * channels * sampleByte;
	char *buf = new char[readSize];
	
	for (;;)
	{
		//一次读取一帧音频
		if (input->bytesReady() < readSize)
		{
			QThread::msleep(1);
			continue;
		}
		int size = 0;
		while (size != readSize)
		{
			int len = io->read(buf + size, readSize - size);
			if (len <= 0) break;
			size += len;
		}

		if (size != readSize) continue;

		//已经读一帧源数据
		//重采样源数据
		const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
		indata[0] = (uint8_t*)buf;
		int len = swr_convert(asc, 
			pcm->data, pcm->nb_samples, //输出参数, 输出存储地址和样本数量
			indata, pcm->nb_samples);
		cout << len << " ";

	}
	delete buf;


	return a.exec();
}
