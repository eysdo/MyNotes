#include "XResample.h"
extern "C"
{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}
#include <iostream>
using namespace std;

#pragma comment(lib, "swresample.lib")

void XResample::Close()
{
	_mux.lock();
	if (_actx)
		swr_free(&_actx);
	_mux.unlock();
}

// ����������������һ�³��˲�����ʽ, ���ΪS16, ���ͷ�codecpar
bool XResample::Open(AVCodecParameters *codecpar, bool isClearPara)
{
	if (!codecpar) return false;
	_mux.lock();
	// ��Ƶ�ز��� �����ĳ�ʼ��
	/*if(!_actx)
		_actx = swr_alloc();*/
	// ���actxΪNULL���Զ�����ռ�
	_actx = swr_alloc_set_opts(_actx,
		av_get_default_channel_layout(2), // ˫����(������)
		(AVSampleFormat)outFormat,        // ���������ʽ 1 AV_SAMPLE_FMT_S16
		codecpar->sample_rate,            // ���������
		av_get_default_channel_layout(codecpar->channels), // �����ʽ
		(AVSampleFormat)codecpar->format,
		codecpar->sample_rate,
		0, 0
	);
	if(isClearPara)
		avcodec_parameters_free(&codecpar);
	int re = swr_init(_actx);
	_mux.unlock();
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "swr_init failed! :" << buf << endl;
		return false;
	}
	//unsigned char *pcm = NULL;

	return true;
}

// �����ز������С, ���ܳɹ�����ͷ�indata�ռ�
int XResample::Resample(AVFrame *indata, unsigned char *data)
{
	if (!indata) return 0;
	if (!data)
	{
		av_frame_free(&indata);
		return 0;
	}
	uint8_t *datas[2] = { 0 };
	datas[0] = data;
	int re = swr_convert(_actx,
		datas, indata->nb_samples,                   // ���
		(const uint8_t**)indata->data, indata->nb_samples  // ����
	);
	if (re <= 0) return re;
	int outSize = re * indata->channels * av_get_bytes_per_sample((AVSampleFormat)outFormat);
	return outSize;
}

XResample::XResample()
{
}


XResample::~XResample()
{
}
