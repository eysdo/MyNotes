/**
 * CPP-RTMP���� - ���ӳٷ���
 * Author: Dejan  QQ: 673008865
 */

#include <iostream>
using namespace std;
extern "C" {
	/* ����FFMPEG APIͷ�ļ� */
#include <libavformat/avformat.h>
#include <libavutil/time.h>
}

/* ����� */
#pragma comment(lib, "avformat.lib") // ��ʽ��װ��
#pragma comment(lib, "avcodec.lib")  // ������
#pragma comment(lib, "avutil.lib")   // ���߿�, ������ȡ������Ϣ��av_usleep()��


#define AV_CODEC_FLAG_GLOBAL_HEADER (1 << 22)
#define CODEC_FLAG_GLOBAL_HEADER AV_CODEC_FLAG_GLOBAL_HEADER

// �������
int avError(int errNum);

int main()
{
	const char* inUrl = "F:/video/2_bd.mp4"; // mp4 or flv
	const char* outUrl = "rtmp://192.168.163.131/live/dejan";

	/////////////////////////////////////////////////////
	/// 1. ��ʼ��
	// ��ʼ�����еķ�װ�ͽ��װ flv mp4 mov mp3 ..
	av_register_all();

	// ��ʼ�������
	avformat_network_init();
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	/// 2. ����������
	// �����ļ��ķ�װ������
	AVFormatContext *ictx = NULL;

	// ���ļ�, ����ļ�ͷ
	int ret = avformat_open_input(&ictx, inUrl, 0, 0);
	if (0 != ret)
		return avError(ret);
	cout << "avformat_open_input success!" << endl;

	// ��ȡ��Ƶ��Ƶ����Ϣ, h264 flv
	ret = avformat_find_stream_info(ictx, 0);
	if (0 > ret)
		return avError(ret);
	// ��ӡ��������Ϣ
	av_dump_format(ictx, 0, inUrl, 0);
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	/// 3. ���������
	// ���������������
	AVFormatContext *octx = NULL;
	ret = avformat_alloc_output_context2(&octx, 0, "flv", outUrl);
	if (0 > ret)
		return avError(ret);
	cout << "avformat_alloc_output_context2 success!" << endl;

	cout << "nb_streams : " << ictx->nb_streams << endl; // AVStream����Ĵ�С
	int vstm_index = -1; // ��Ƶ������
	// ������
	for (int i = 0; i < ictx->nb_streams; i++)
	{
		// ���������: ��Ƶ����Ƶ��Ϣ
		AVStream *in_stm = ictx->streams[i];
		// ������ͨ������Ϊ�����ͬʱ��ʼ���ռ�
		AVStream *out_stm = avformat_new_stream(octx, in_stm->codec->codec);
		if (!out_stm)
		{
			cout << "avformat_new_stream() failed!";
			ret = AVERROR_UNKNOWN;
		}
		// ���Ʊ�������������: ���������������� copy to ����������������
		ret = avcodec_parameters_copy(out_stm->codecpar, in_stm->codecpar);
		if (0 > ret)
			cout << "avcodec_parameters_copy() failed!" << endl;
		out_stm->codecpar->codec_tag = 0;
		out_stm->codec->codec_tag = 0;
		if (octx->oformat->flags & AVFMT_GLOBALHEADER)
			out_stm->codec->flags = out_stm->codec->flags | CODEC_FLAG_GLOBAL_HEADER;
		// �����Ƶ������
		if (in_stm->codec->codec_type == AVMEDIA_TYPE_VIDEO && vstm_index == -1)
			vstm_index = i;
	}
	// ��ӡ�������Ϣ
	av_dump_format(octx, 0, outUrl, 1);
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	/// 4. ׼������

	// ��IO
	ret = avio_open(&octx->pb, outUrl, AVIO_FLAG_WRITE);
	if (0 > ret)
		return avError(ret);

	// д��ͷ����Ϣ
	ret = avformat_write_header(octx, 0);
	if (0 > ret)
		return avError(ret);
	cout << "avformat_write_header Success!" << endl;
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	/// 5. ��ʼ����
	// ����ÿһ֡����
	AVPacket pkt;
	long long start_time = av_gettime(); // ��ȡ��ǰ��ʱ���(��λ:΢��)
	long long frame_index = 0; // �Զ�����Ƶ֡����
	for (;;)
	{
		// �����������Ƶ��
		AVStream *in_stm = NULL, *out_stm = NULL;
		// ��ȡ����֡����
		ret = av_read_frame(ictx, &pkt);
		if (0 > ret) break;

		/*
		PTS��Presentation Time Stamp����ʾ����ʱ��
		DTS��Decoding Time Stamp������ʱ��
		*/
		//û����ʾʱ�� (����δ����� H.264)
		if (pkt.pts == AV_NOPTS_VALUE)
		{
			AVRational time_base = ictx->streams[vstm_index]->time_base;

			// ������֮֡���ʱ��
			/*
			r_frame_rate ����֡����
			av_q2d ת��Ϊdouble����
			*/
			int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(ictx->streams[vstm_index]->r_frame_rate);

			// ���ò���
			pkt.pts = (double)(frame_index*calc_duration) / (double)(av_q2d(time_base)*AV_TIME_BASE);
			pkt.dts = pkt.pts;
			pkt.duration = (double)calc_duration / (double)(av_q2d(time_base)*AV_TIME_BASE);
		}

		// ��ʱͬ������, ����ʱ�����Ĺؼ�
		if (pkt.stream_index == vstm_index)
		{
			AVRational time_base = ictx->streams[vstm_index]->time_base;
			AVRational time_base_q = { 1, AV_TIME_BASE };
			// ������Ƶ����ʱ��
			int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
			// ����ʵ����Ƶ�Ĳ���ʱ��
			int64_t now_time = av_gettime() - start_time;

			AVRational avr = ictx->streams[vstm_index]->time_base;
			cout << avr.num << " " << avr.den << "  " << pkt.dts << "  " << pkt.pts << "   " << pts_time << endl;
			if (pts_time > now_time)
			{
				// ˯��һ��ʱ�䣨Ŀ�����õ�ǰ��Ƶ��¼�Ĳ���ʱ����ʵ��ʱ��ͬ����
				av_usleep((unsigned int)(pts_time - now_time));
			}
		}

		in_stm = ictx->streams[pkt.stream_index];
		out_stm = octx->streams[pkt.stream_index];

		// ������ʱ������ָ��ʱ���
		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stm->time_base, out_stm->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stm->time_base, out_stm->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.duration = (int)av_rescale_q(pkt.duration, in_stm->time_base, out_stm->time_base);
		// �ֽ�����λ�ã�-1 ��ʾ��֪���ֽ���λ��
		pkt.pos = -1;

		if (pkt.stream_index == vstm_index)
		{
			printf("Send %8d video frames to output URL\n", frame_index);
			frame_index++;
		}

		// ����֡����
		ret = av_interleaved_write_frame(octx, &pkt);
		if (0 != ret)
			return avError(ret);

		//�ͷ� packet
		av_free_packet(&pkt);
	}


	getchar();
	return 0;
}

int avError(int errNum) {
	char buf[1024];
	//��ȡ������Ϣ
	av_strerror(errNum, buf, sizeof(buf));
	cout << " failed! " << buf << endl;
	getchar();
	return -1;
}