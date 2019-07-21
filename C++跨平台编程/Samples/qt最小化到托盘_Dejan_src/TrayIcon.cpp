#include "TrayIcon.h"


TrayIcon::TrayIcon(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 创建系统托盘图标
	createSysTrayIcon();
}

void TrayIcon::closeEvent(QCloseEvent *event)
{
	// 最小化到托盘阻止关闭进程
	if (mSysTrayIcon->isVisible()) 
	{
		// 隐藏窗口
		this->hide();

		// 气泡提示
		//显示消息球，1s后自动消失
		//第一个参数是标题
		//第二个参数是消息内容
		//第三个参数图标
		//第四个参数是超时毫秒数
		mSysTrayIcon->showMessage(QString::fromLocal8Bit("Message Title"),
		QString::fromLocal8Bit("程序已在后台运行!"),
		QSystemTrayIcon::Information,
		1000);
	}
	event->ignore();
}

/* 创建系统托盘图标 */
void TrayIcon::createSysTrayIcon()
{
	//新建QSystemTrayIcon对象
	if (!mSysTrayIcon) 
	{
		mSysTrayIcon = new QSystemTrayIcon(this);
		//新建托盘要显示的icon
		QIcon icon = QIcon(":/TrayIcon/re/copy_d.png");
		//将icon设到QSystemTrayIcon对象中
		mSysTrayIcon->setIcon(icon);
		//当鼠标移动到托盘上的图标时，会显示此处设置的内容
		mSysTrayIcon->setToolTip(QString::fromLocal8Bit("测试最小化到托盘!"));
		//给QSystemTrayIcon添加槽函数 - 图标被点击事件
		connect(mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

		//建立托盘操作的菜单
		createActions();
		createMenu();

		//在系统托盘显示此对象
		mSysTrayIcon->show();
	}
}

/* 最小化托盘图标被点击事件 */
void TrayIcon::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		//单击托盘图标

		break;
	case QSystemTrayIcon::DoubleClick:
		//双击托盘图标
		//双击后显示主程序窗口
		this->show();
		break;
	default:
		break;
	}
}

/* 最小化托盘图标右键菜单 - 选择项功能 */
void TrayIcon::createActions()
{
	mShowMainAction = new QAction(QString::fromLocal8Bit("显示主界面"), this);
	connect(mShowMainAction, SIGNAL(triggered()), this, SLOT(on_showMainAction()));

	mExitAppAction = new QAction(QString::fromLocal8Bit("退出"), this);
	connect(mExitAppAction, SIGNAL(triggered()), this, SLOT(on_exitAppAction()));
}

/* 创建右键菜单 */
void TrayIcon::createMenu()
{
	mMenu = new QMenu(this);
	//新增菜单项---显示主界面
	mMenu->addAction(mShowMainAction);
	//增加分隔符
	mMenu->addSeparator();
	//新增菜单项---退出程序
	mMenu->addAction(mExitAppAction);
	//把QMenu赋给QSystemTrayIcon对象
	mSysTrayIcon->setContextMenu(mMenu);
}

/* 最小化托盘图标右键菜单项 - 显示主界面 */
void TrayIcon::on_showMainAction()
{
	this->show();
}

/* 最小化托盘图标右键菜单项 - 退出 */
void TrayIcon::on_exitAppAction()
{
	exit(0);
}
