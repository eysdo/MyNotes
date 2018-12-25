#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
extern "C"
{
	#include <libavformat/avformat.h>
	#include <libavutil/time.h>
}
using namespace std;
#ifdef _MSC_VER // Windows
	#pragma comment(lib, "avformat.lib")
	#pragma comment(lib, "avutil.lib")
	#pragma comment(lib, "avcodec.lib")
#elif __GNUC__ // Linux

#endif

void print_err(int& re)
{
	if (0 != re)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf));
		cout << buf << endl;
		getchar();
	}
}

int main()
{
	const char* inUrl = "C:/Users/Dejan/Videos/test.flv";
	const char* outUrl = "rtmp://192.168.163.131/live";
	// ��ʼ�����еķ�װ�ͽ��װ flv mp4 mov mp3 ..
	av_register_all();

	// ��ʼ�������
	avformat_network_init();
	///////////////////////////////////////////////////////////
	// 1.���ļ�, ���װ
	// �����װ������
	AVFormatContext *ictx = NULL;

	// ���ļ�, ����ļ�ͷ
	int re = avformat_open_input(&ictx, inUrl, 0, 0);
	if (0 != re)
	{
		print_err(re);
		return -1;
	}
	cout << "open file " << inUrl << "Success" << endl;

	// ��ȡ��Ƶ��Ƶ����Ϣ, h264 flv
	re = avformat_find_stream_info(ictx, 0);
	if (0 != re)
	{
		print_err(re);
		return -1;
	}

	av_dump_format(ictx, 0, inUrl, 0);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// �����

	// ���������������
	AVFormatContext *octx = NULL;
	avformat_alloc_output_context2(&octx, 0, "flv", outUrl);
	if (!octx)
	{
		print_err(re);
		return -1;
	}
	cout << "octx create success!" << endl;

	// ���������
	// ���������AVStream
	for (int i = 0; i < ictx->nb_streams; i++)
	{
		// ���������
		AVStream *out = avformat_new_stream(octx, ictx->streams[i]->codec->codec);
		if (!out)
		{
			printf("error: avformat_new_stream");
			return -1;
		}
		// -����������Ϣ, ����MP4
		//re = avcodec_copy_context(out->codec, ictx->streams[i]->codec);
		// -End
		re = avcodec_parameters_copy(out->codecpar, ictx->streams[i]->codecpar);
		out->codec->codec_tag = 0;
	}
	av_dump_format(octx, 0, outUrl, 1);


	//////////////////////////////////////////////////////

	// rtmp����

	// ��IO
	re = avio_open(&octx->pb, outUrl, AVIO_FLAG_WRITE);
	if (!octx->pb)
	{
		print_err(re);
		return -1;
	}

	// д��ͷ��Ϣ
	re = avformat_write_header(octx, 0);
	if (re < 0)
	{
		print_err(re);
		return -1;
	}
	cout << "avformat_write_header" << re << endl;

	// ����ÿһ֡����
	AVPacket pkt;
	for (;;)
	{
		re = av_read_frame(ictx, &pkt);
		if (re != 0)
		{
			break;
		}
		cout << pkt.pts << " " <<flush;
		// ����ת��pts dts
		AVRational itime = ictx->streams[pkt.stream_index]->time_base;
		AVRational otime = octx->streams[pkt.stream_index]->time_base;
		pkt.pts = av_rescale_q_rnd(pkt.pts, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.pts, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q_rnd(pkt.duration, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.pos = -1;

		re = av_interleaved_write_frame(octx, &pkt);
		if (pkt.stream_index == 0)
		{
			av_usleep(30*1000);
		}
		if (re < 0)
		{
			print_err(re);
			return -1;
		}
		//av_packet_unref(&pkt);
	}



	cout << "file to rtmp test" << endl;
	getchar();
	return 0;
}