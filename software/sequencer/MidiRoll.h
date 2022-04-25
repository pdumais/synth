#pragma once
#include <QVector>

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QRubberBand>

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

Q_DECLARE_METATYPE(MidiRollEvent*)

class MidiRoll : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MidiRoll(QWidget *parent = nullptr);

    enum MouseMode{
        Edit,
        Select
    };

    void setTicksPerQuarterNotes(int tpqn);
    void setEvents(QVector<MidiRollEvent*>& eventsList);
    void addEvent(MidiRollEvent* event);
    void setTrack(int track);
    void setCursorPosition(int tick);
    void setMouseMode(MouseMode mode);
    void deleteSelection();

signals:
    void onRowHover(int row);
    void onNewBlock(int tick, int row, int lenght);
    void onBlockRemoved(MidiRollEvent* ev);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


private:
    QGraphicsScene *scene;
    QGraphicsLineItem* cursorItem;
    QVector<MidiRollEvent*> eventsList;
    QGraphicsRectItem* newBlock;
    int ticksPerQuarterNote;
    int track;
    int cursorPosition;
    bool adding;
    QRubberBand* rubberBand;
    MouseMode mode;

    void setup();
    void createGrid();
    void updateEvents();
    void createEventBlock(MidiRollEvent*);
    void updateNewBlockRect(QPointF pos);
    void updateSelection();
    void initNewBlock();
};

