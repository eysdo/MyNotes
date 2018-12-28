#include <QtCore/QCoreApplication>
#include <iostream>
#include <QThread>
#include "XMediaEncode.h"
#include "XRtmp.h"
#include "XAudioRecord.h"
#include "XVideoCapture.h"
using namespace std;
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	const char *outUrl = "rtmp://192.168.163.131/live";
	int ret = 0;
	int sampleRate = 44100;
	int channels = 2;
	int sampleByte = 2;
	int nbSamples = 1024;
	///�������
	XVideoCapture *xv = XVideoCapture::Get();
	if (!xv->Init(0))
	{
		printf("open camera failed!\n");
		getchar();
		return -1;
	}
	printf("open camera success!\n");
	xv->Start();


	///1 qt��Ƶ��ʼ¼��
	XAudioRecord *ar = XAudioRecord::Get();
	ar->sampleRate = sampleRate;
	ar->channels = channels;
	ar->sampleByte = sampleByte;
	ar->nbSamples = nbSamples;
	if (!ar->Init())
	{
		printf("XAudioRecord Init() failed!\n");
		getchar();
		return -1;
	}
	ar->Start();

	///����Ƶ������
	XMediaEncode *xe = XMediaEncode::Get();

	/// 2 ��ʼ����ʽת��������
	/// 3 ��ʼ����������ݽṹ
	xe->inWidth = xv->width;
	xe->inHeight = xv->height;
	xe->outWidth = xv->width;
	xe->outHeight = xv->height;
	if (!xe->InitScale())
	{
		getchar();
		return -1;
	}
	printf("��ʼ����Ƶ����ת�������ĳɹ�!\n");

	///2 ��Ƶ�ز��� �����ĳ�ʼ��
	xe->channels = channels;
	xe->nbSamples = nbSamples;
	xe->sampleRate = sampleRate;
	xe->inSampleFmt = XSampleFMT::X_S16;
	xe->outSampleFmt = XSampleFMT::X_FLTP;
	if (!xe->InitResample())
	{
		getchar();
		return -1;
	}

	///4 ��ʼ����Ƶ������
	if (!xe->InitAudioCodec())
	{
		getchar();
		return -1;
	}
	/// ��ʼ����Ƶ������
	if (!xe->InitVideoCodec())
	{
		getchar();
		return -1;
	}

	///5 �����װ������Ƶ������
	// a ���������װ��������
	XRtmp *xr = XRtmp::Get(0);
	if (!xr->Init(outUrl))
	{
		getchar();
		return -1;
	}

	// b �����Ƶ��
	int vindex = 0;
	vindex = xr->AddStream(xe->vc);
	if (vindex < 0)
	{
		getchar();
		return -1;
	}

	// b �����Ƶ��
	int aindex = xr->AddStream(xe->ac);
	if (aindex < 0)
	{
		getchar();
		return -1;
	}

	///6 ��rtmp���������IO
	// д���װͷ
	if (!xr->SendHead())
	{
		getchar();
		return -1;
	}


	//һ�ζ�ȡһ֡��Ƶ���ֽ���
	for (;;)
	{
		//һ�ζ�ȡһ֡��Ƶ
		XData ad = ar->Pop();
		XData vd = xv->Pop();
		if (ad.size <= 0 && vd.size <= 0)
		{
			QThread::msleep(1);
			continue;
		}

		//������Ƶ
		if (ad.size > 0)
		{
			//�ز���Դ����
			AVFrame *pcm = xe->Resample(ad.data);
			ad.Drop();
			AVPacket *pkt = xe->EncodeAudio(pcm);
			if (pkt)
			{
				//// ����
				if (xr->SendFrame(pkt, aindex))
				{
					cout << "#" << flush;
				}
			}
			
		}

		//������Ƶ
		if (vd.size > 0)
		{
			AVFrame *yuv = xe->RGBToYUV(vd.data);
			vd.Drop();
			AVPacket *pkt = xe->EncodeVideo(yuv);
			if (pkt)
			{
				//// ����
				if (xr->SendFrame(pkt, vindex))
				{
					cout << "@" << flush;
				}
			}
		}

		

	}
	

	getchar();
	return a.exec();
}
