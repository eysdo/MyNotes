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

	// 最小化托盘图标句柄
	QSystemTrayIcon *mSysTrayIcon = 0;

protected:
	void closeEvent(QCloseEvent *event);

private:
	// 创建系统托盘图标
	void createSysTrayIcon();

	// 系统托盘右键菜单
	QMenu* mMenu = 0;
	QAction* mShowMainAction;
	QAction* mExitAppAction;
	void createActions();
	void createMenu();


private slots:
	// 最小化托盘图标被点击事件
	void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

	// 系统托盘右键菜单
	void on_showMainAction();
	void on_exitAppAction();


private:
	Ui::TrayIconClass ui;
};
