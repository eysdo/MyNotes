#pragma once

#include <QtWidgets/QWidget>
#include "ui_XPlay2.h"

class XPlay2 : public QWidget
{
	Q_OBJECT

public:
	// ˫��ȫ��
	void mouseDoubleClickEvent(QMouseEvent *e);

	// ���ڳߴ�仯
	void resizeEvent(QResizeEvent *e);

	// ��ʱ�� ��������ʾ
	void timerEvent(QTimerEvent *e);

	XPlay2(QWidget *parent = Q_NULLPTR);
	~XPlay2();

public slots:
	void OpenFile();

private:
	Ui::XPlay2Class ui;
};
