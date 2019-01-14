#include "XVideoUI.h"
#include <QFileDialog>
#include <string>
#include <iostream>
#include <QMessageBox>
#include "XVideoThread.h"
#include "XFilter.h"
using namespace std;
static bool pressSlider = false;

XVideoUI::XVideoUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	// ���ش��ڶ���������
	setWindowFlags(Qt::FramelessWindowHint);
	// Qtԭ���ǲ�֧��cv::Mat��Ϊ�źźͲ۵Ĵ��ݲ���, ����������Ҫע��һ�²���ʹ��
	qRegisterMetaType<cv::Mat>("cv::Mat");

	//ԭ��Ƶ��ʾ�ź�
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1,
		SLOT(SetImage(cv::Mat))
	);

	//�����Ƶ��ʾ�ź�
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewDes(cv::Mat)),
		ui.des,
		SLOT(SetImage(cv::Mat))
	);

	startTimer(40);
}

void XVideoUI::Open()
{
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��ѡ����Ƶ�ļ�"));
	if (name.isEmpty()) return;
	string file = name.toLocal8Bit().data();
	//QMessageBox::information(0, "", name);
	//cout << file << endl;
	if (!XVideoThread::Get()->Open(file))
	{
		cout << "open file failed!" << endl;
		return;
	}
}

void XVideoUI::timerEvent(QTimerEvent *e)
{
	if (pressSlider) return;
	double pos = XVideoThread::Get()->GetPos();
	ui.playSlider->setValue(pos*1000);
}

void XVideoUI::SliderPress()
{
	pressSlider = true;
}

void XVideoUI::SliderRelease()
{
	pressSlider = false;
}

// �������϶�
void XVideoUI::SetPos(int pos)
{
	XVideoThread::Get()->Seek((double)pos / 1000.);
}

void XVideoUI::Set()
{
	XFilter::Get()->Clear();
	// �ԱȶȺ�����
	if (ui.bright->value() > 0 || 
		ui.contrast->value() >1)
	{
		XFilter::Get()->Add(XTask{XTASK_GAIN,
		{(double)ui.bright->value(), ui.contrast->value()}
		});
	}
}