#pragma once
#include <QVector>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include "MidiRollFrameBase.h"


class MidiRoll : public MidiRollFrameBase
{
    Q_OBJECT
public:
    explicit MidiRoll(QWidget *parent = nullptr);

    void setTicksPerQuarterNotes(int tpqn);
    void setEvents(QVector<MidiRollEvent*>& eventsList);
    void addEvent(MidiRollEvent* event);
    void updateEvent(MidiRollEvent* original, MidiRollEvent* updated);
    void setTrack(int track);
    void setCursorPosition(int tick);
    void deleteSelection();
    QRect getSection();

signals:
    void onRowHover(int row);
    void onNewBlock(int tick, int row, int lenght);
    void onBlockRemoved(MidiRollEvent* ev);
    void onBlocksMoved(std::vector<MidiRollEvent*> originals, std::vector<MidiRollEvent*> changed);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void handleNewBlockCreated(long ticks, int note) override;
    void handleBlocksMoved(std::vector<MidiRollEvent*>, std::vector<MidiRollEvent*>) override;
    void handleHover(int note) override;

private:
    // Graphics Items
    QGraphicsScene *scene;
    QGraphicsLineItem* cursorItem;
    QVector<MidiRollEvent*> eventsList;
    QGraphicsItemGroup* gridLines;

    // Settings
    int track;
    int cursorPosition;

    void setup();
    void updateGridWidth();
    void createGrid();
    void createSection();
    void updateSection(int start, int end);
    void updateEvents();
    void createEventBlock(MidiRollEvent*);
    void updateNewBlockRect(QPointF pos);
    void updateSelection();

};

