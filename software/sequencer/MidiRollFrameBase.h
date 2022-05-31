#pragma once

#include <QGraphicsView>
#include <functional>
#include <QRubberBand>

#define QUARTERNOTE_WIDTH 40
#define QUARTERNOTE_HEIGHT 15


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

class MidiRollState
{
public:
    std::function<void(QMouseEvent*)> onMousePressed;
    std::function<void(QMouseEvent*)> onMouseMove;
    std::function<void(QMouseEvent*)> onMouseReleased;
};



class MidiRollFrameBase: public QGraphicsView
{
    Q_OBJECT
public:
    enum MouseMode{
        Edit,
        Select
    };

    explicit MidiRollFrameBase(QWidget *parent = nullptr);
    virtual ~MidiRollFrameBase();

    virtual void handleHover(int note) = 0;
    virtual void handleNewBlockCreated(long ticks, int note) = 0;
    virtual void handleBlocksMoved(std::vector<MidiRollEvent*>, std::vector<MidiRollEvent*>) = 0;


    void setMouseMode(MouseMode mode);

protected:
    int ticksPerQuarterNote;
    long sectionStart;
    long sectionEnd;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void setup();
    void initNewBlock();

    QGraphicsLineItem* sectionStartItem;
    QGraphicsLineItem* sectionEndItem;
private:

    // States
    MidiRollState* state;
    MidiRollState* idle;
    MidiRollState* selecting;
    MidiRollState* adding;
    MidiRollState* moving;

    // State dependent vars
    QGraphicsRectItem* newBlock;
    QGraphicsItemGroup *selectionGroup;
    QRubberBand* rubberBand;

    MouseMode mode;

    void updateSelection();
    void updateNewBlockRect(QPointF spos);
};

