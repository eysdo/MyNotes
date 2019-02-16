#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include "XDecode.h"
#include "XResample.h"
#include <QThread>
#include "XAudioPlay.h"
#include "XAudioThread.h"
#include "XVideoThread.h"
#include <iostream>
using namespace std;

class TestThread :public QThread
{
public:
	void Init()
	{
		demux.Open("C:/Users/Dejan/Videos/test.mp4");
		demux.Read();
		demux.Clear();
		demux.Close();
		demux.Open("C:/Users/Dejan/Videos/test.mp4");
		//cout << "CopyVPara = " << demux.CopyVPara() << endl;
		//cout << "CopyAPara = " << demux.CopyAPara() << endl;
		//cout << "seek = " << demux.Seek(0.95) << endl;

		//cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
		//vdecode.Clear();
		//vdecode.Close();
		/*
		cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
		cout << "resample.Open() = " << resample.Open(demux.CopyAPara()) << endl;
		XAudioPlay::Get()->_channels = demux._channels;
		XAudioPlay::Get()->_sampleRate = demux._sampleRate;

		cout << "XAudioPlay::Get()->Open() = " << XAudioPlay::Get()->Open() << endl;
		*/

		cout << "aThread.Open = " << aThread.Open(demux.CopyAPara(), demux._sampleRate, demux._channels);
		vThread.Open(demux.CopyVPara(), video, demux._width, demux._height);
		aThread.start();
		vThread.start();
	}
	void run()
	{
		for (;;)
		{
			AVPacket *pkt = demux.Read();
			if (demux.IsAudio(pkt))
			{
				aThread.Push(pkt);
				/*adecode.Send(pkt);
				AVFrame *frame = adecode.Recv();
				int len = resample.Resample(frame, pcm);
				cout<<"Resample:"<< len <<" ";
				while (len > 0)
				{
					if (XAudioPlay::Get()->GetFree() >= len)
					{
						XAudioPlay::Get()->Write(pcm, len);
						break;
					}
					msleep(1);
				}*/
				//cout << "Audio: " << frame << endl;
			}
			else
			{
				vThread.Push(pkt);
				/*
				vdecode.Send(pkt);
				AVFrame *frame = vdecode.Recv();
				video->Repaint(frame);
				*/
				//msleep(40);
				//cout << "Video: " << frame << endl;
			}
			if (!pkt)break;
		}
	}
public:
	XDemux demux;
	//XDecode vdecode;
	//XDecode adecode;
	//XResample resample;
	XVideoWidget *video = 0;
	//unsigned char *pcm = new unsigned char[1024 * 1024];
	XAudioThread aThread;
	XVideoThread vThread;
};

#include "XDemuxThread.h"
int main(int argc, char *argv[])
{
	//TestThread tt;
	

	QApplication a(argc, argv);
	XPlay2 w;
	w.show();

	// ³õÊ¼»¯gl´°¿Ú
	//w.ui.video->Init(tt.demux._width, tt.demux._height);
	//tt.video = w.ui.video;
	//tt.Init();
	//tt.start();
	//XDemuxThread dt;
	// rtmp://live.hkstv.hk.lxdns.com/live/hks1
	// C:/Users/Dejan/Videos/test.mp4
	// https://gslb.miaopai.com/stream/vmij5lN2X1rnMHAw5lhXM8R0lcBnhv1oYv6kww__.mp4
	// http://tb-video.bdstatic.com/tieba-smallvideo/19_be8d169db43666b2a6d92a8336bc1390.mp4
	//dt.Open("C:/Users/Dejan/Videos/test.mp4", w.ui.video);
	//dt.Start();

	return a.exec();
}
