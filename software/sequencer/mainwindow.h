#pragma once

#include <QMainWindow>
#include <QTimer>
#include "Sequencer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_playButton_clicked();

private:
    Ui::MainWindow *ui;
    Sequencer *sequencer;
    QTimer *timer;
    std::vector<std::map<long, MidiEvent*>> midiFileEvents;

    void changeTrack(int trk);
    void processTimer();
    void setBPM(int bpm);
};
