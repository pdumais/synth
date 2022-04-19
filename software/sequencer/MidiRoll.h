#pragma once
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsLineItem>

class MidiRollEvent
{
public:
    MidiRollEvent(int start, int length, int channel, int note)
    {
        this->length = length;
        this->start = start;
        this->channel = channel;
        this->note = note;
    }

    int length;
    int start;
    int channel;
    int note;
};

class MidiRoll : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MidiRoll(QWidget *parent = nullptr);

    void setTicksPerQuarterNotes(int tpqn);
    void setEvents(QVector<MidiRollEvent*>& eventsList);
    void setTrack(int track);
    void setCursorPosition(int tick);

signals:

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene *scene;
    QGraphicsLineItem* cursorItem;
    QVector<MidiRollEvent*> eventsList;
    int ticksPerQuarterNote;
    int track;
    int cursorPosition;

    void setup();
    void createGrid();
    void updateEvents();
};

