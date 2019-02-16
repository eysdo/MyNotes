#include "XDemux.h"
#include <iostream>
using namespace std;
extern "C" {
#include <libavformat/avformat.h>
}
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")

// ���ת�������ǲο�opencv����
static double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

bool XDemux::Open(const char *url)
{
	Close();
	// ��������
	AVDictionary *opts = NULL;
	// ����rtsp����tcpЭ���
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);
	// ������ʱʱ��
	av_dict_set(&opts, "max_delay", "500", 0);

	_mux.lock();
	int re = avformat_open_input(
		&_ic,
		url,
		0,    // 0��ʾ�Զ�ѡ����װ��
		&opts // ��������, ����rtsp����ʱʱ��
	);
	if (re != 0)
	{
		_mux.unlock();
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "open " << url << " failed! :" << buf << endl;
		return false;
	}
	cout << "open " << url << " success!" << endl;

	// ��ȡ����Ϣ
	re = avformat_find_stream_info(_ic, 0);

	// ��ʱ�� ����
	_totalMs = _ic->duration / (AV_TIME_BASE / 1000);
	cout << "totalMs = " << _totalMs << endl;

	// ��ӡ��Ƶ����ϸ��Ϣ
	av_dump_format(_ic, 0, url, 0);

	// ��ȡ��Ƶ��
	_videoStream = av_find_best_stream(_ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	AVStream *vs = _ic->streams[_videoStream];
	cout << "============================="<<_videoStream << "��Ƶ��Ϣ" << "=============================" << endl;
	cout << "format = " << vs->codecpar->format << endl;
	cout << "codec_id = " << vs->codecpar->codec_id << endl;
	cout << "width = " << vs->codecpar->width << endl;
	cout << "height = " << vs->codecpar->height << endl;
	// ֡�� fps ����ת��
	cout << "video fps = " << r2d(vs->avg_frame_rate) << endl;


	// ��ȡ��Ƶ��
	_audioStream = av_find_best_stream(_ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	AVStream *as = _ic->streams[_audioStream];
	cout << "=============================" << _audioStream << "��Ƶ��Ϣ" << "=============================" << endl;
	cout << "format = " << as->codecpar->format << endl;
	cout << "codec_id = " << as->codecpar->codec_id << endl;
	cout << "sample_rate = " << as->codecpar->sample_rate << endl;
	cout << "channels = " << as->codecpar->channels << endl;
	// һ֡����?? ��ͨ��������
	cout << "frame_size = " << as->codecpar->frame_size << endl;
	// ��ͨ��������Ϊ1024,˫ͨ��һ֡���� = 1024 * 2 * 2 = 4096 (��ͨ��������*ͨ����*��������ռ���ֽ�)
	// fps = sample_rate / frame_size (������/��ͨ��������)
	_mux.unlock();

	return true;
}

// �ռ���Ҫ�������ͷ�, �ͷ�AVPacket����ռ� �� ���ݿռ� av_packet_free
AVPacket* XDemux::Read()
{
	_mux.lock();
	if (!_ic) // �ݴ�
	{
		_mux.unlock();
		return false;
	}
	AVPacket *pkt = av_packet_alloc();
	// ��ȡһ֡, ������ռ�
	int re = av_read_frame(_ic, pkt);
	if (re != 0)
	{
		_mux.unlock();
		av_packet_free(&pkt);
		return 0;
	}
	// ptsת��Ϊ����
	pkt->pts = pkt->pts * (r2d(_ic->streams[pkt->stream_index]->time_base) * 1000);
	pkt->dts = pkt->dts * (r2d(_ic->streams[pkt->stream_index]->time_base) * 1000);
	_mux.unlock();
	cout << "pkt->pts:" << pkt->pts << " "<<flush;
	return pkt;
}

// ��ȡ��Ƶ����  ���صĿռ���Ҫ���� avcodec_parameters_free()
AVCodecParameters* XDemux::CopyVPara()
{
	_mux.lock();
	if (!_ic)
	{
		_mux.unlock();
		return NULL;
	}
	AVCodecParameters *pa = avcodec_parameters_alloc();
	avcodec_parameters_copy(pa, _ic->streams[_videoStream]->codecpar);
	_mux.unlock();
	return pa;
}

// ��ȡ��Ƶ����  ���صĿռ���Ҫ���� avcodec_parameters_free()
AVCodecParameters* XDemux::CopyAPara()
{
	_mux.lock();
	if (!_ic)
	{
		_mux.unlock();
		return NULL;
	}
	AVCodecParameters *pa = avcodec_parameters_alloc();
	avcodec_parameters_copy(pa, _ic->streams[_audioStream]->codecpar);
	_mux.unlock();
	return pa;
}

// seek λ�� pos 0.0~1.0
bool XDemux::Seek(double pos)
{
	_mux.lock();
	if (!_ic)
	{
		_mux.unlock();
		return false;
	}
	// �����ȡ����
	avformat_flush(_ic);
	long long seekPos = 0;
	seekPos = _ic->streams[_videoStream]->duration * pos;
	int re = av_seek_frame(_ic, _videoStream, seekPos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME); // AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME  �Ӻ��濪ʼ���ҳ��ؼ�֡
	_mux.unlock();
	if (re < 0) return false;
	return true;
}

// ��ն�ȡ����
void XDemux::Clear()
{
	_mux.lock();
	if (!_ic)
	{
		_mux.unlock();
		return;
	}
	// �����ȡ����
	avformat_flush(_ic);
	_mux.unlock();
}

void XDemux::Close()
{
	_mux.lock();
	if (!_ic)
	{
		_mux.unlock();
		return;
	}
	avformat_close_input(&_ic);
	// ý����ʱ�� (����)
	_totalMs = 0;
	_mux.unlock();
}

XDemux::XDemux()
{
	static bool isFirst = true;
	static std::mutex dmux;
	dmux.lock();
	if (isFirst)
	{
		// ��ʼ����װ��
		av_register_all();

		// ��ʼ������� (���Դ�rtsp rtmp http Э�����Ƶ)
		avformat_network_init();
	}
	dmux.unlock();

}


XDemux::~XDemux()
{
}
