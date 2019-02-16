#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include "XDecode.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	XDemux demux;
	demux.Open("C:/Users/Dejan/Videos/test.mp4");
	demux.Read();
	demux.Clear();
	demux.Close();
	demux.Open("C:/Users/Dejan/Videos/test.mp4");
	cout << "CopyVPara = " << demux.CopyVPara() << endl;
	cout << "CopyAPara = " << demux.CopyAPara() << endl;
	cout << "seek = " << demux.Seek(0.95) << endl;

	XDecode vdecode;
	cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
	//vdecode.Clear();
	//vdecode.Close();
	XDecode adecode;
	cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
	
	for (;;)
	{
		AVPacket *pkt = demux.Read();
		if (demux.IsAudio(pkt))
		{
			adecode.Send(pkt);
			AVFrame *frame = adecode.Recv();
			cout << "Audio: " << frame << endl;
		}
		else
		{
			vdecode.Send(pkt);
			AVFrame *frame = vdecode.Recv();
			cout << "Video: " << frame << endl;
		}
		if (!pkt)break;
	}


	QApplication a(argc, argv);
	XPlay2 w;
	w.show();
	return a.exec();
}
