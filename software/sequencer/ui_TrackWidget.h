/********************************************************************************
** Form generated from reading UI file 'TrackWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRACKWIDGET_H
#define UI_TRACKWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_TrackWidget
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;

    void setupUi(QFrame *TrackWidget)
    {
        if (TrackWidget->objectName().isEmpty())
            TrackWidget->setObjectName(QString::fromUtf8("TrackWidget"));
        TrackWidget->resize(185, 39);
        TrackWidget->setMinimumSize(QSize(185, 39));
        TrackWidget->setCursor(QCursor(Qt::PointingHandCursor));
        label = new QLabel(TrackWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 69, 19));
        label_2 = new QLabel(TrackWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(90, 10, 20, 20));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(10, 10));
        label_2->setBaseSize(QSize(100, 100));
        label_2->setAutoFillBackground(false);
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);"));
        label_2->setFrameShape(QFrame::Box);
        label_3 = new QLabel(TrackWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(120, 10, 20, 20));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(10, 10));
        label_3->setBaseSize(QSize(10, 10));
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 0);"));
        label_3->setFrameShape(QFrame::Box);
        pushButton = new QPushButton(TrackWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(150, 10, 20, 20));

        retranslateUi(TrackWidget);

        QMetaObject::connectSlotsByName(TrackWidget);
    } // setupUi

    void retranslateUi(QFrame *TrackWidget)
    {
        TrackWidget->setWindowTitle(QApplication::translate("TrackWidget", "Frame", nullptr));
        label->setText(QApplication::translate("TrackWidget", "TextLabel", nullptr));
        label_2->setText(QString());
        label_3->setText(QString());
        pushButton->setText(QApplication::translate("TrackWidget", "P", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrackWidget: public Ui_TrackWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACKWIDGET_H
