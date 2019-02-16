#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include "XDecode.h"
#include "XResample.h"
#include <QThread>
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
		cout << "CopyVPara = " << demux.CopyVPara() << endl;
		cout << "CopyAPara = " << demux.CopyAPara() << endl;
		//cout << "seek = " << demux.Seek(0.95) << endl;

		cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
		//vdecode.Clear();
		//vdecode.Close();
		cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
		cout << "resample.Open() = " << resample.Open(demux.CopyAPara()) << endl;

	}
	void run()
	{
		for (;;)
		{
			AVPacket *pkt = demux.Read();
			if (demux.IsAudio(pkt))
			{
				adecode.Send(pkt);
				AVFrame *frame = adecode.Recv();
				cout<<"Resample:"<<resample.Resample(frame, pcm)<<" ";
				//cout << "Audio: " << frame << endl;
			}
			else
			{
				vdecode.Send(pkt);
				AVFrame *frame = vdecode.Recv();
				video->Repaint(frame);
				msleep(40);
				//cout << "Video: " << frame << endl;
			}
			if (!pkt)break;
		}
	}
public:
	XDemux demux;
	XDecode vdecode;
	XDecode adecode;
	XResample resample;
	XVideoWidget *video;
	unsigned char *pcm = new unsigned char[1024 * 1024];
};

int main(int argc, char *argv[])
{
	TestThread tt;
	tt.Init();



	QApplication a(argc, argv);
	XPlay2 w;
	w.show();

	// ³õÊ¼»¯gl´°¿Ú
	w.ui.video->Init(tt.demux._width, tt.demux._height);
	tt.video = w.ui.video;
	tt.start();

	return a.exec();
}
