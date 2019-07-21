#include "TrayIcon.h"


TrayIcon::TrayIcon(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// ����ϵͳ����ͼ��
	createSysTrayIcon();
}

void TrayIcon::closeEvent(QCloseEvent *event)
{
	// ��С����������ֹ�رս���
	if (mSysTrayIcon->isVisible()) 
	{
		// ���ش���
		this->hide();

		// ������ʾ
		//��ʾ��Ϣ��1s���Զ���ʧ
		//��һ�������Ǳ���
		//�ڶ�����������Ϣ����
		//����������ͼ��
		//���ĸ������ǳ�ʱ������
		mSysTrayIcon->showMessage(QString::fromLocal8Bit("Message Title"),
		QString::fromLocal8Bit("�������ں�̨����!"),
		QSystemTrayIcon::Information,
		1000);
	}
	event->ignore();
}

/* ����ϵͳ����ͼ�� */
void TrayIcon::createSysTrayIcon()
{
	//�½�QSystemTrayIcon����
	if (!mSysTrayIcon) 
	{
		mSysTrayIcon = new QSystemTrayIcon(this);
		//�½�����Ҫ��ʾ��icon
		QIcon icon = QIcon(":/TrayIcon/re/copy_d.png");
		//��icon�赽QSystemTrayIcon������
		mSysTrayIcon->setIcon(icon);
		//������ƶ��������ϵ�ͼ��ʱ������ʾ�˴����õ�����
		mSysTrayIcon->setToolTip(QString::fromLocal8Bit("������С��������!"));
		//��QSystemTrayIcon��Ӳۺ��� - ͼ�걻����¼�
		connect(mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

		//�������̲����Ĳ˵�
		createActions();
		createMenu();

		//��ϵͳ������ʾ�˶���
		mSysTrayIcon->show();
	}
}

/* ��С������ͼ�걻����¼� */
void TrayIcon::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		//��������ͼ��

		break;
	case QSystemTrayIcon::DoubleClick:
		//˫������ͼ��
		//˫������ʾ�����򴰿�
		this->show();
		break;
	default:
		break;
	}
}

/* ��С������ͼ���Ҽ��˵� - ѡ����� */
void TrayIcon::createActions()
{
	mShowMainAction = new QAction(QString::fromLocal8Bit("��ʾ������"), this);
	connect(mShowMainAction, SIGNAL(triggered()), this, SLOT(on_showMainAction()));

	mExitAppAction = new QAction(QString::fromLocal8Bit("�˳�"), this);
	connect(mExitAppAction, SIGNAL(triggered()), this, SLOT(on_exitAppAction()));
}

/* �����Ҽ��˵� */
void TrayIcon::createMenu()
{
	mMenu = new QMenu(this);
	//�����˵���---��ʾ������
	mMenu->addAction(mShowMainAction);
	//���ӷָ���
	mMenu->addSeparator();
	//�����˵���---�˳�����
	mMenu->addAction(mExitAppAction);
	//��QMenu����QSystemTrayIcon����
	mSysTrayIcon->setContextMenu(mMenu);
}

/* ��С������ͼ���Ҽ��˵��� - ��ʾ������ */
void TrayIcon::on_showMainAction()
{
	this->show();
}

/* ��С������ͼ���Ҽ��˵��� - �˳� */
void TrayIcon::on_exitAppAction()
{
	exit(0);
}
