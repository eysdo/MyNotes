/********************************************************************************
** Form generated from reading UI file 'testQtOpengl.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTQTOPENGL_H
#define UI_TESTQTOPENGL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include "XVideoWidget.h"

QT_BEGIN_NAMESPACE

class Ui_testQtOpenglClass
{
public:
    XVideoWidget *openGLWidget;

    void setupUi(QWidget *testQtOpenglClass)
    {
        if (testQtOpenglClass->objectName().isEmpty())
            testQtOpenglClass->setObjectName(QStringLiteral("testQtOpenglClass"));
        testQtOpenglClass->resize(799, 599);
        openGLWidget = new XVideoWidget(testQtOpenglClass);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(0, 0, 800, 600));

        retranslateUi(testQtOpenglClass);

        QMetaObject::connectSlotsByName(testQtOpenglClass);
    } // setupUi

    void retranslateUi(QWidget *testQtOpenglClass)
    {
        testQtOpenglClass->setWindowTitle(QApplication::translate("testQtOpenglClass", "testQtOpengl", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class testQtOpenglClass: public Ui_testQtOpenglClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTQTOPENGL_H
