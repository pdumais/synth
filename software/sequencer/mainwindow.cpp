#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSpacerItem>
#include <QInputDialog>
#include "MidiFileUtil.h"
#include "MidiRoll.h"
#include <QFileDialog>
#include <QDir>

#define MAX_TRACKS 16

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    for (int i = 0; i<MAX_TRACKS; i++)
    {
        TrackWidget *tw = new TrackWidget();
        this->ui->frame_4->layout()->addWidget(tw);
        tw->setLabel("Track " + QString::number(i));
        this->trackWidgets.append(tw);
        connect(tw, &TrackWidget::trackSelected, this, &MainWindow::on_track_selected);
    }

    this->zoomx = 1;
    this->zoomy = 1;
    this->defaultChannel = 0;

    this->timer = new QTimer(this);
    connect(this->timer, &QTimer::timeout, this, &MainWindow::processTimer);

    this->sequencer = new Sequencer();



    MidiRoll *mr = ui->midiRoll;
    if (QCoreApplication::arguments().size()>1)
    {
        this->loadFile(QCoreApplication::arguments().at(1).toStdString());
    }
    this->ui->midiRoll->resetMatrix();
    this->ui->midiRoll->scale(this->zoomx, this->zoomy);

    this->setBPM(120);

    connect(mr, &MidiRoll::onRowHover, this, &MainWindow::on_midiRoll_row_hover);
    connect(mr, &MidiRoll::onNewBlock, this, &MainWindow::on_midiRoll_NewBlock);
    connect(mr, &MidiRoll::onBlockRemoved, this, &MainWindow::on_midiRoll_BlockRemoved);
    connect(mr, &MidiRoll::onBlocksMoved, this, &MainWindow::on_midiroll_BlocksMoved);

    this->noteStatusLabel = new QLabel();
    this->ui->statusbar->addWidget(this->noteStatusLabel);
    this->modeLabel = new QLabel();
    this->ui->statusbar->addWidget(this->modeLabel);



}

void MainWindow::loadFile(std::string fname)
{
    this->sequencer->reset();
    MidiRoll *mr = ui->midiRoll;
    MidiFileUtil mf(fname);
    auto midiFileEvents = mf.getEventsFromFile();

    int i = 0;
    for (auto trk : midiFileEvents)
    {
        this->sequencer->loadEvents(i, trk);
        i++;
    }
    mr->setTicksPerQuarterNotes(mf.getTicksPerQuarteNotes());
    sequencer->setTicksPerQuarterNote(mf.getTicksPerQuarteNotes());
    this->changeTrack(0);

    mr->setCursorPosition(2*mf.getTicksPerQuarteNotes());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->sequencer;
    for (auto tw : this->trackWidgets) delete tw;
}

void MainWindow::processTimer()
{
    QRect section = this->ui->midiRoll->getSection();
    if (this->sequencer->getCurrentTick() > (section.width()+section.x()))
    {
        this->sequencer->setPosition(section.x());
    }

    this->sequencer->onClockTick();

    ui->midiRoll->setCursorPosition(this->sequencer->getCurrentTick());
}

void MainWindow::setBPM(int bpm)
{
    this->timer->start((1000/(bpm/60))/this->sequencer->getTicksPerQuarterNote());
}

void MainWindow::changeTrack(int trk)
{
    this->sequencer->setTrack(trk);

    MidiRoll *mr = ui->midiRoll;
    QVector<MidiRollEvent*> el;

    for (auto mev : this->sequencer->dumpEvents())
    {
        for (auto ev : mev.second)
        {
            if (ev->event == 0x90)
            {
                el.append(new MidiRollEvent(ev->start, ev->duration, ev->channel, ev->d1));
            }
        }
    }
    mr->setEvents(el);

    for (int i = 0; i<MAX_TRACKS; i++)
    {
        this->trackWidgets[i]->setActive(i==trk);
    }
}


void MainWindow::saveMidiFile(std::string fname)
{
    std::vector<std::map<long, std::vector<MidiEvent*>>> tracks;
    for (int i = 0; i < MAX_TRACKS; i++)
    {
        auto trk = this->sequencer->dumpEvents(i);
        if (trk.size() == 0) continue;

        tracks.push_back(trk);
    }
    MidiFileUtil::saveToFile(fname, tracks, this->sequencer->getTicksPerQuarterNote());

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    this->defaultChannel = index;
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    this->setBPM(arg1);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    this->zoomx = ((float)value/100.0);
    this->ui->midiRoll->resetMatrix();
    this->ui->midiRoll->scale(this->zoomx, this->zoomy);
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    this->zoomy = ((float)value/100.0);
    this->ui->midiRoll->resetMatrix();
    this->ui->midiRoll->scale(this->zoomx, this->zoomy);
}

void MainWindow::on_midiRoll_row_hover(int row)
{
    this->noteStatusLabel->setNum(row);
}

void MainWindow::on_midiRoll_NewBlock(int tick, int row, int length)
{
    this->ui->midiRoll->addEvent(new MidiRollEvent(tick, length, this->defaultChannel, row));

    MidiEvent *ev = new MidiEvent();
    ev->start = tick;
    ev->duration = length;
    ev->channel = this->defaultChannel;
    ev->event = 0x90;
    ev->d1 = row;
    ev->d2 = 127;
    this->sequencer->addEvent(tick, ev);

}


void MainWindow::on_actionEdit_2_triggered() // Play
{
    this->sequencer->play();
}

void MainWindow::on_actionEdit_3_triggered() // Set Edit Mode
{
    this->ui->midiRoll->setMouseMode(MidiRoll::MouseMode::Edit);
    this->modeLabel->setText("Editing");
}

void MainWindow::on_actionSelect_triggered() // Set Select Mode
{
    this->ui->midiRoll->setMouseMode(MidiRoll::MouseMode::Select);
    this->modeLabel->setText("Selecting");
}

void MainWindow::on_actionStop_triggered()
{
    this->sequencer->stop();
}

void MainWindow::on_actionDelete_triggered()
{
    this->ui->midiRoll->deleteSelection();
}

void MainWindow::on_midiRoll_BlockRemoved(MidiRollEvent* ev)
{
    this->sequencer->removeEvent(ev->start, ev->channel, ev->note);

}

void MainWindow::on_track_selected(TrackWidget* track)
{
    if (track->isActive()) return; // Don't change if it's the same track

    int i = 0;
    while (i < this->trackWidgets.size())
    {
        if (this->trackWidgets[i] == track)
        {
            this->on_actionStop_triggered();
            this->changeTrack(i);
            return;
        }
        i++;
    }
}

void MainWindow::showDevicesDialog()
{
    auto devs = this->sequencer->getMidi()->listDevices();
    QStringList items;
    for (std::string d : devs) items << QString(d.c_str());

    bool ok;
    QString item = QInputDialog::getItem(this, "Midi Devices", "Device", items, 0, false, &ok);

    if (!ok) return;

    int index = items.indexOf(item);
    this->sequencer->getMidi()->useDevice(index);
    //TODO: display device name on status bar
}

void MainWindow::on_midiroll_BlocksMoved(std::vector<MidiRollEvent*> originals, std::vector<MidiRollEvent*> changed)
{
    for (int i = 0; i < originals.size(); i++)
    {
        auto o = originals[i];
        auto c = changed[i];
        this->ui->midiRoll->updateEvent(o, c);

        MidiEvent evo(o->start, o->length, o->channel, 0x90, o->note, 0);
        MidiEvent evc(c->start, c->length, c->channel, 0x90, c->note, 0);
        this->sequencer->updateEvent(&evo, &evc);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog qfd;

    QString fname = qfd.getOpenFileName(this, "Select Midi file", QDir().absolutePath(),"*.mid");
    if (!fname.isEmpty())
    {
        this->loadFile(fname.toStdString());
    }

}

void MainWindow::on_actionSave_As_triggered()
{
    QFileDialog qfd;

    QString fname = qfd.getSaveFileName(this, "Select Midi file", QDir().absolutePath(),"*.mid");
    if (!fname.isEmpty())
    {
        this->saveMidiFile(fname.toStdString());
    }

}
