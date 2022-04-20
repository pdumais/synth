#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MidiFileUtil.h"
#include "MidiRoll.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->timer = new QTimer(this);
    connect(this->timer, &QTimer::timeout, this, &MainWindow::processTimer);

    this->sequencer = new Sequencer();

    MidiFileUtil mf("C:\\Users\\pat\\Documents\\EAGLE\\projects\\synth\\ice.mid");
    //MidiFileUtil mf("/home/pat/projects/synth/tracks/sonic/ice.mid");

    MidiRoll *mr = ui->midiRoll;
    this->midiFileEvents = mf.getEventsFromFile();
    mr->setTicksPerQuarterNotes(mf.getTicksPerQuarteNotes());
    sequencer->setTicksPerQuarterNote(mf.getTicksPerQuarteNotes());
    this->changeTrack(0);

    mr->setCursorPosition(2*mf.getTicksPerQuarteNotes());
    this->setBPM(120);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->sequencer;
}

void MainWindow::processTimer()
{
    this->sequencer->onClockTick();

    ui->midiRoll->setCursorPosition(this->sequencer->getCurrentTick());
}

void MainWindow::setBPM(int bpm)
{
    this->timer->start(bpm*1000/60/this->sequencer->getTicksPerQuarterNote());
}

void MainWindow::changeTrack(int trk)
{
    this->sequencer->stop();
    this->sequencer->loadEvents(midiFileEvents[trk]);

    MidiRoll *mr = ui->midiRoll;
    QVector<MidiRollEvent*> el;
    for (auto mev : this->midiFileEvents[trk])
    {
        auto ev = mev.second;

        while (ev)
        {
            if (ev->event == 0x90)
            {
                el.append(new MidiRollEvent(ev->start, ev->duration, ev->channel, ev->d1));
            }
            ev = ev->next;
        }
    }
    mr->setEvents(el);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    this->changeTrack(index);
}

void MainWindow::on_playButton_clicked()
{
    this->sequencer->play();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    this->setBPM(arg1);
}

void MainWindow::on_stopButton_clicked()
{
    this->sequencer->stop();
}
