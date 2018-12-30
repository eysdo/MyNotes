/********************************************************************************
** Form generated from reading UI file 'xrtmpstreamer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XRTMPSTREAMER_H
#define UI_XRTMPSTREAMER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XRtmpStreamerClass
{
public:
    QLineEdit *outUrl;
    QLabel *label;
    QLineEdit *inUrl;
    QLabel *label_2;
    QPushButton *startButton;
    QLabel *label_3;
    QComboBox *face;

    void setupUi(QWidget *XRtmpStreamerClass)
    {
        if (XRtmpStreamerClass->objectName().isEmpty())
            XRtmpStreamerClass->setObjectName(QStringLiteral("XRtmpStreamerClass"));
        XRtmpStreamerClass->resize(600, 232);
        outUrl = new QLineEdit(XRtmpStreamerClass);
        outUrl->setObjectName(QStringLiteral("outUrl"));
        outUrl->setGeometry(QRect(170, 50, 391, 31));
        label = new QLabel(XRtmpStreamerClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 50, 111, 31));
        inUrl = new QLineEdit(XRtmpStreamerClass);
        inUrl->setObjectName(QStringLiteral("inUrl"));
        inUrl->setGeometry(QRect(170, 110, 391, 31));
        label_2 = new QLabel(XRtmpStreamerClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 110, 121, 31));
        startButton = new QPushButton(XRtmpStreamerClass);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setGeometry(QRect(410, 170, 151, 51));
        label_3 = new QLabel(XRtmpStreamerClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 170, 121, 31));
        face = new QComboBox(XRtmpStreamerClass);
        face->setObjectName(QStringLiteral("face"));
        face->setGeometry(QRect(170, 170, 111, 31));

        retranslateUi(XRtmpStreamerClass);
        QObject::connect(startButton, SIGNAL(clicked()), XRtmpStreamerClass, SLOT(Stream()));

        QMetaObject::connectSlotsByName(XRtmpStreamerClass);
    } // setupUi

    void retranslateUi(QWidget *XRtmpStreamerClass)
    {
        XRtmpStreamerClass->setWindowTitle(QApplication::translate("XRtmpStreamerClass", "XRtmpStreamer", Q_NULLPTR));
        outUrl->setText(QApplication::translate("XRtmpStreamerClass", "rtmp://192.168.163.131/live", Q_NULLPTR));
        label->setText(QApplication::translate("XRtmpStreamerClass", "\346\216\250\346\265\201RTMP URL:", Q_NULLPTR));
        inUrl->setText(QApplication::translate("XRtmpStreamerClass", "0", Q_NULLPTR));
        label_2->setText(QApplication::translate("XRtmpStreamerClass", "\350\276\223\345\205\245\347\232\204URL(0\346\210\2261):", Q_NULLPTR));
        startButton->setText(QApplication::translate("XRtmpStreamerClass", "\345\274\200\345\247\213\346\216\250\346\265\201", Q_NULLPTR));
        label_3->setText(QApplication::translate("XRtmpStreamerClass", "\347\276\216\351\242\234\347\272\247\345\210\253:", Q_NULLPTR));
        face->clear();
        face->insertItems(0, QStringList()
         << QApplication::translate("XRtmpStreamerClass", "\344\270\200\347\272\247", Q_NULLPTR)
         << QApplication::translate("XRtmpStreamerClass", "\344\272\214\347\272\247", Q_NULLPTR)
         << QApplication::translate("XRtmpStreamerClass", "\344\270\211\347\272\247", Q_NULLPTR)
        );
    } // retranslateUi

};

namespace Ui {
    class XRtmpStreamerClass: public Ui_XRtmpStreamerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XRTMPSTREAMER_H
