#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MidiFileUtil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    /*std::cout << "test";
    this->sequencer = new Sequencer();
    for (auto &it : this->sequencer->dumpEvents()) {
        std::cout << it << "\r\n";
    }*/

    this->timer = new QTimer(this);
    connect(this->timer, &QTimer::timeout, this, &MainWindow::processTimer);
    this->setBPM(120);
    this->timer->start();

    MidiFileUtil mf;
    auto m = mf.getEventsFromFile("/home/pat/clip1/clip1-Track_1-1.mid");
    this->sequencer->loadEvents(m);

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->sequencer;
}

void MainWindow::processTimer()
{
    this->sequencer->onClockTick();
}

void MainWindow::setBPM(int bpm)
{
    this->timer->setInterval(bpm*60*1000);
}
