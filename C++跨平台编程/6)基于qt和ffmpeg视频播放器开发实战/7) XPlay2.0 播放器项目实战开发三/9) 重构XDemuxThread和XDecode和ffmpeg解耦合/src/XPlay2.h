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

	// ��ͣ
	void SetPause(bool isPause);

	XPlay2(QWidget *parent = Q_NULLPTR);
	~XPlay2();

public slots:
	void OpenFile();
	void PlayOrPause();
	void SliderPress();
	void SliderRelease();

private:
	Ui::XPlay2Class ui;
	bool _isSliderPress = false;
};
