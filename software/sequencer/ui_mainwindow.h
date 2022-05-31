/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "MidiRoll.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionEdit_2;
    QAction *actionEdit_3;
    QAction *actionSelect;
    QAction *actionStop;
    QAction *actionDelete;
    QAction *actionOpen;
    QAction *actionSave_As;
    QWidget *centralwidget;
    QGridLayout *gridLayout_4;
    QFrame *frame_3;
    QGridLayout *gridLayout_3;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QSlider *horizontalSlider;
    MidiRoll *midiRoll;
    QSlider *verticalSlider;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QSpinBox *spinBox;
    QMenuBar *menubar;
    QMenu *menutest;
    QMenu *menuEdit;
    QMenu *menuMode;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1521, 844);
        actionEdit_2 = new QAction(MainWindow);
        actionEdit_2->setObjectName(QString::fromUtf8("actionEdit_2"));
        actionEdit_3 = new QAction(MainWindow);
        actionEdit_3->setObjectName(QString::fromUtf8("actionEdit_3"));
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName(QString::fromUtf8("actionSelect"));
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy);
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        frame_2 = new QFrame(frame_3);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy1);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSlider = new QSlider(frame_2);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMinimum(10);
        horizontalSlider->setMaximum(100);
        horizontalSlider->setSingleStep(10);
        horizontalSlider->setValue(100);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSlider, 1, 1, 1, 1);

        midiRoll = new MidiRoll(frame_2);
        midiRoll->setObjectName(QString::fromUtf8("midiRoll"));
        sizePolicy.setHeightForWidth(midiRoll->sizePolicy().hasHeightForWidth());
        midiRoll->setSizePolicy(sizePolicy);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::NoBrush);
        midiRoll->setBackgroundBrush(brush);
        midiRoll->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_2->addWidget(midiRoll, 0, 1, 1, 1);

        verticalSlider = new QSlider(frame_2);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setMinimum(10);
        verticalSlider->setMaximum(100);
        verticalSlider->setSingleStep(10);
        verticalSlider->setValue(100);
        verticalSlider->setOrientation(Qt::Vertical);

        gridLayout_2->addWidget(verticalSlider, 0, 0, 1, 1);

        frame_4 = new QFrame(frame_2);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setMinimumSize(QSize(200, 0));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        gridLayout_2->addWidget(frame_4, 0, 2, 1, 1);


        gridLayout_3->addWidget(frame_2, 1, 0, 1, 1);

        frame = new QFrame(frame_3);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBox = new QComboBox(frame);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout->addWidget(comboBox, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        spinBox = new QSpinBox(frame);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(60);
        spinBox->setMaximum(1000);
        spinBox->setValue(120);

        gridLayout->addWidget(spinBox, 0, 0, 1, 1);


        gridLayout_3->addWidget(frame, 0, 0, 1, 1);


        gridLayout_4->addWidget(frame_3, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1521, 24));
        menutest = new QMenu(menubar);
        menutest->setObjectName(QString::fromUtf8("menutest"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuMode = new QMenu(menuEdit);
        menuMode->setObjectName(QString::fromUtf8("menuMode"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menutest->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menutest->addAction(actionEdit_2);
        menutest->addAction(actionStop);
        menuEdit->addAction(menuMode->menuAction());
        menuEdit->addAction(actionDelete);
        menuMode->addAction(actionEdit_3);
        menuMode->addAction(actionSelect);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave_As);

        retranslateUi(MainWindow);

        comboBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionEdit_2->setText(QApplication::translate("MainWindow", "Play", nullptr));
#ifndef QT_NO_SHORTCUT
        actionEdit_2->setShortcut(QApplication::translate("MainWindow", "P", nullptr));
#endif // QT_NO_SHORTCUT
        actionEdit_3->setText(QApplication::translate("MainWindow", "Edit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionEdit_3->setShortcut(QApplication::translate("MainWindow", "E", nullptr));
#endif // QT_NO_SHORTCUT
        actionSelect->setText(QApplication::translate("MainWindow", "Select", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSelect->setShortcut(QApplication::translate("MainWindow", "S", nullptr));
#endif // QT_NO_SHORTCUT
        actionStop->setText(QApplication::translate("MainWindow", "Stop", nullptr));
#ifndef QT_NO_SHORTCUT
        actionStop->setShortcut(QApplication::translate("MainWindow", "X", nullptr));
#endif // QT_NO_SHORTCUT
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", nullptr));
#ifndef QT_NO_SHORTCUT
        actionDelete->setShortcut(QApplication::translate("MainWindow", "Del", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As ...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave_As->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        comboBox->setItemText(0, QApplication::translate("MainWindow", "0", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindow", "1", nullptr));
        comboBox->setItemText(2, QApplication::translate("MainWindow", "2", nullptr));
        comboBox->setItemText(3, QApplication::translate("MainWindow", "3", nullptr));
        comboBox->setItemText(4, QApplication::translate("MainWindow", "4", nullptr));
        comboBox->setItemText(5, QApplication::translate("MainWindow", "5", nullptr));
        comboBox->setItemText(6, QApplication::translate("MainWindow", "6", nullptr));
        comboBox->setItemText(7, QApplication::translate("MainWindow", "7", nullptr));
        comboBox->setItemText(8, QApplication::translate("MainWindow", "8", nullptr));
        comboBox->setItemText(9, QApplication::translate("MainWindow", "9", nullptr));
        comboBox->setItemText(10, QApplication::translate("MainWindow", "10", nullptr));
        comboBox->setItemText(11, QApplication::translate("MainWindow", "11", nullptr));
        comboBox->setItemText(12, QApplication::translate("MainWindow", "12", nullptr));
        comboBox->setItemText(13, QApplication::translate("MainWindow", "13", nullptr));
        comboBox->setItemText(14, QApplication::translate("MainWindow", "14", nullptr));
        comboBox->setItemText(15, QApplication::translate("MainWindow", "15", nullptr));

        comboBox->setCurrentText(QApplication::translate("MainWindow", "0", nullptr));
        menutest->setTitle(QApplication::translate("MainWindow", "Actions", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
        menuMode->setTitle(QApplication::translate("MainWindow", "Mode", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
