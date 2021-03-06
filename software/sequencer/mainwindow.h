#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include "Sequencer.h"
#include "TrackWidget.h"
#include "MidiRoll.h"

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

    void on_spinBox_valueChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_verticalSlider_valueChanged(int value);

    void on_midiRoll_row_hover(int row);

    void on_midiRoll_NewBlock(int tick, int row, int lenght);

    void on_midiRoll_BlockRemoved(MidiRollEvent* ev);

    void on_actionEdit_2_triggered();

    void on_actionEdit_3_triggered();

    void on_actionSelect_triggered();

    void on_actionStop_triggered();

    void on_actionDelete_triggered();

    void on_track_selected(TrackWidget* track);

    void on_midiroll_BlocksMoved(std::vector<MidiRollEvent*> originals, std::vector<MidiRollEvent*> changed);

    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

private:
    Ui::MainWindow *ui;
    Sequencer *sequencer;
    QTimer *timer;
    QLabel *noteStatusLabel;
    QLabel *modeLabel;
    QVector<TrackWidget*> trackWidgets;
    float zoomx;
    float zoomy;
    int defaultChannel;

    void loadFile(std::string fname);
    void changeTrack(int trk);
    void processTimer();
    void setBPM(int bpm);

    void showDevicesDialog();
    void saveMidiFile(std::string fname);

};
