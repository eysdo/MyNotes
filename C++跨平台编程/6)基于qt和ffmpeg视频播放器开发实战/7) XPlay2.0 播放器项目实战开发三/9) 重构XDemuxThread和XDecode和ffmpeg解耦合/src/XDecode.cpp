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
	// 清理解码缓冲
	if (_codec)
		avcodec_flush_buffers(_codec);
	_mux.unlock();
}

// 打开解码器
bool XDecode::Open(AVCodecParameters *codecpar)
{
	if (!codecpar) return false;
	Close();

	///////////////////////////////////////////////////////
	/// 视频解码器打开
	/// 找到解码器
	AVCodec *vcodec = avcodec_find_decoder(codecpar->codec_id);
	if (!vcodec)
	{
		avcodec_parameters_free(&codecpar);
		cout << "can't find the codec id " << codecpar->codec_id << endl;
		return false;
	}
	cout << "find the AVCodec " << codecpar->codec_id << endl;

	_mux.lock();
	/// 创建解码器上下文
	_codec = avcodec_alloc_context3(vcodec);

	/// 配置解码器上下文参数
	avcodec_parameters_to_context(_codec, codecpar);
	avcodec_parameters_free(&codecpar);

	// 八线程解码
	_codec->thread_count = 8;

	/// 打开解码器上下文
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

// 发送到解码线程, 不管成功与否都释放pkt空间(对象和媒体内容)
bool XDecode::Send(AVPacket *pkt)
{
	// 容错处理
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

// 获取解码数据, 一次send可能需要多次Recv
// 每次复制一份, 由调用者释放 av_frame_free()
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
