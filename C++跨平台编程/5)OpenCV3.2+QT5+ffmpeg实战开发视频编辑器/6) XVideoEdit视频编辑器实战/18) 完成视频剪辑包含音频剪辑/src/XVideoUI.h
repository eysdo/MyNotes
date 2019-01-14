#pragma once

#include <QtWidgets/QWidget>
#include "ui_XVideoUI.h"

class XVideoUI : public QWidget
{
	Q_OBJECT

public:
	XVideoUI(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent *e);

public slots:
	void Open();
	void Play();
	void Pause();

	void SliderPress();
	void SliderRelease();
	// �������϶�
	void SetPos(int);
	void Left(int pos);
	void Right(int pos);

	// ���ù�����
	void Set();
	// ������Ƶ
	void Export();

	// ��������
	void ExportEnd();

	// ˮӡ
	void Mark();

	// �ں�
	void Blend();

	// �ϲ�
	void Merge();

	

private:
	Ui::XVideoUIClass ui;
};
