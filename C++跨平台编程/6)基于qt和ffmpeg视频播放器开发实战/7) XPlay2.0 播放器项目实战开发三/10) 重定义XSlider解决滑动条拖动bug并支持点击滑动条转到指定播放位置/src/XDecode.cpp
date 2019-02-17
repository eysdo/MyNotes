#include "XDecode.h"
extern "C"
{
#include <libavcodec/avcodec.h>
}
#include <iostream>
using namespace std;

void XFreePacket(AVPacket **pkt)
{
	if (!pkt || !(*pkt)) return;
	av_packet_free(pkt);
}

void XFreeFrame(AVFrame **frame)
{
	if (!frame || !(*frame)) return;
	av_frame_free(frame);
}

void XDecode::Close()
{
	_mux.lock();
	if (_codec)
	{
		avcodec_close(_codec);
		avcodec_free_context(&_codec);
	}
	_pts = 0;
	_mux.unlock();
}

void XDecode::Clear()
{
	_mux.lock();
	// ������뻺��
	if (_codec)
		avcodec_flush_buffers(_codec);
	_mux.unlock();
}

// �򿪽�����
bool XDecode::Open(AVCodecParameters *codecpar)
{
	if (!codecpar) return false;
	Close();

	///////////////////////////////////////////////////////
	/// ��Ƶ��������
	/// �ҵ�������
	AVCodec *vcodec = avcodec_find_decoder(codecpar->codec_id);
	if (!vcodec)
	{
		avcodec_parameters_free(&codecpar);
		cout << "can't find the codec id " << codecpar->codec_id << endl;
		return false;
	}
	cout << "find the AVCodec " << codecpar->codec_id << endl;

	_mux.lock();
	/// ����������������
	_codec = avcodec_alloc_context3(vcodec);

	/// ���ý����������Ĳ���
	avcodec_parameters_to_context(_codec, codecpar);
	avcodec_parameters_free(&codecpar);

	// ���߳̽���
	_codec->thread_count = 8;

	/// �򿪽�����������
	int re = avcodec_open2(_codec, 0, 0);
	if (re != 0)
	{
		avcodec_free_context(&_codec);
		_mux.unlock();
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "avcodec_open2 failed! :" << buf << endl;
		return false;
	}
	_mux.unlock();
	cout << "avcodec_open2 success!" << endl;
	return true;
}

// ���͵������߳�, ���ܳɹ�����ͷ�pkt�ռ�(�����ý������)
bool XDecode::Send(AVPacket *pkt)
{
	// �ݴ���
	if (!pkt || pkt->size <= 0 || !pkt->data) return false;
	_mux.lock();
	if (!_codec)
	{
		_mux.unlock();
		return false;
	}
	int re = avcodec_send_packet(_codec, pkt);
	_mux.unlock();
	av_packet_free(&pkt);
	if (re != 0) return false;
	return true;

}

// ��ȡ��������, һ��send������Ҫ���Recv
// ÿ�θ���һ��, �ɵ������ͷ� av_frame_free()
AVFrame* XDecode::Recv()
{
	_mux.lock();
	if (!_codec)
	{
		_mux.unlock();
		return NULL;
	}
	AVFrame *frame = av_frame_alloc();
	int re = avcodec_receive_frame(_codec, frame);
	_mux.unlock();
	if (re != 0)
	{
		av_frame_free(&frame);
		return NULL;
	}
	//cout << "["<<frame->linesize[0] << "] " << flush;
	_pts = frame->pts;
	return frame;
}

XDecode::XDecode()
{
}


XDecode::~XDecode()
{
}
