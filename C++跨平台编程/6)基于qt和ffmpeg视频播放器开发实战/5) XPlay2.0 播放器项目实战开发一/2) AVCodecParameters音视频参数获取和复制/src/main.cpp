#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	XDemux demux;
	demux.Open("C:/Users/Dejan/Videos/test.mp4");
	cout << "CopyVPara = " << demux.CopyVPara() << endl;
	cout << "CopyAPara = " << demux.CopyAPara() << endl;
	
	for (;;)
	{
		AVPacket *pkt = demux.Read();
		if (!pkt)break;
	}


	QApplication a(argc, argv);
	XPlay2 w;
	w.show();
	return a.exec();
}
