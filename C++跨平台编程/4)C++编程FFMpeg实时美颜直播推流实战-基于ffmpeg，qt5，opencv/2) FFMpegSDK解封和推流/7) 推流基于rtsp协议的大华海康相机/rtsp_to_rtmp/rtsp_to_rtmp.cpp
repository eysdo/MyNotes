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
	}
}

static double r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}

int main()
{
	const char* inUrl = "rtsp://admin:dejan1314520@192.168.25.101:554/onvif1";
	const char* outUrl = "rtmp://192.168.163.131/live";
	// ��ʼ�����еķ�װ�ͽ��װ flv mp4 mov mp3 ..
	av_register_all();

	// ��ʼ�������
	avformat_network_init();
	///////////////////////////////////////////////////////////
	// 1.���ļ�, ���װ
	// �����װ������
	AVFormatContext *ictx = NULL;

	// ����rtspЭ����ʱ���ֵ
	AVDictionary *opts = NULL;
	char key[] = "max_delay";
	char val[] = "500";
	av_dict_set(&opts, key, val, 0);
	char key2[] = "rtsp_transport";
	char val2[] = "tcp";
	av_dict_set(&opts, key2, val2, 0);

	// ��rtsp��, ����ļ�ͷ
	int re = avformat_open_input(&ictx, inUrl, 0, &opts);
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
	long long startTime = av_gettime(); // ΢��ʱ���
	for (;;)
	{
		re = av_read_frame(ictx, &pkt);
		if (re != 0 || pkt.size <= 0)
		{
			continue;
		}

		cout << pkt.pts << " " <<flush;
		// ����ת��pts dts
		AVRational itime = ictx->streams[pkt.stream_index]->time_base;
		AVRational otime = octx->streams[pkt.stream_index]->time_base;
		pkt.pts = av_rescale_q_rnd(pkt.pts, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.pts, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q_rnd(pkt.duration, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.pos = -1;
		
		/*
		// ��Ƶ֡�����ٶ�
		if (ictx->streams[pkt.stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			AVRational tb = ictx->streams[pkt.stream_index]->time_base;
			long long now = av_gettime() - startTime;
			long long dts = 0;
			dts = pkt.dts * (1000 * 1000 * r2d(tb));
			if(dts > now)
				av_usleep(dts-now);
		}
		*/

		re = av_interleaved_write_frame(octx, &pkt);
		
		if (re < 0)
		{
			print_err(re);
		}
		//av_packet_unref(&pkt);
	}



	cout << "file to rtmp test" << endl;
	getchar();
	return 0;
}