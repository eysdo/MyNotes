#pragma once

#include <QtWidgets/QWidget>
#include "ui_TrayIcon.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>

class TrayIcon : public QWidget
{
	Q_OBJECT

public:
	TrayIcon(QWidget *parent = Q_NULLPTR);

	// ��С������ͼ����
	QSystemTrayIcon *mSysTrayIcon = 0;

protected:
	void closeEvent(QCloseEvent *event);

private:
	// ����ϵͳ����ͼ��
	void createSysTrayIcon();

	// ϵͳ�����Ҽ��˵�
	QMenu* mMenu = 0;
	QAction* mShowMainAction;
	QAction* mExitAppAction;
	void createActions();
	void createMenu();


private slots:
	// ��С������ͼ�걻����¼�
	void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

	// ϵͳ�����Ҽ��˵�
	void on_showMainAction();
	void on_exitAppAction();


private:
	Ui::TrayIconClass ui;
};
