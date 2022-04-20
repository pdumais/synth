#include "MidiRoll.h"
#include <QLabel>
#include <QGraphicsRectItem>

#define QUARTERNOTE_WIDTH 40
#define QUARTERNOTE_HEIGHT 15

QColor cols[] = {Qt::red, Qt::blue, Qt::green, Qt::yellow};


//TODO: move horizontal (With onChangeEvent)
//TODO: move vertical
//TODO: resize
//TODO: add (with onNewEvent)
//TODO: delete (with onDeleteEvent)

MidiRoll::MidiRoll(QWidget *parent) : QGraphicsView(parent)
{
    this->ticksPerQuarterNote = 192;
    this->track = 0;
    this->cursorPosition = 0;

    this->setup();
}
void MidiRoll::setTicksPerQuarterNotes(int tpqn)
{
    this->ticksPerQuarterNote = tpqn;
}

void MidiRoll::setEvents(QVector<MidiRollEvent*>& events)
{
    //TODO: delete old events
    if (events.size() == 0) return;

    this->eventsList = events;
    auto lastev = this->eventsList.last();

    this->scene->setSceneRect(0,0,((float)(lastev->start+lastev->length)/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH, 128*QUARTERNOTE_HEIGHT);
    this->updateEvents();
}

void MidiRoll::updateEvents()
{

    this->scene->clear();
    this->scene->update();

    for (auto &ev : this->eventsList)
    {
        int x = ((float)ev->start/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH;
        int y = ((127-ev->note)*QUARTERNOTE_HEIGHT);
        int w = ((float)ev->length/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH;
        int h = (QUARTERNOTE_HEIGHT-1);

        QColor col = QColor(cols[ev->channel]);

        this->scene->addRect(x, y, w, h, QPen(col), col);
    }

    this->createGrid();
}

void MidiRoll::setTrack(int track)
{
    this->track = track;
    this->updateEvents();
}

void MidiRoll::setCursorPosition(int tick)
{
    int h = this->sceneRect().height();
    int x = ((float)tick/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH;
    this->cursorPosition = x;

    this->cursorItem->setLine(x, 0, x, h-1);
}

void MidiRoll::createGrid()
{

    int tickw = QUARTERNOTE_WIDTH;
    int tickh = QUARTERNOTE_HEIGHT;
    int w = this->sceneRect().width();
    int h = this->sceneRect().height();


    // Each square is a quarternote
    for (int i=tickw; i<w; i+=tickw)
    {
        this->scene->addLine(i,0,i,h-1);
    }

    for (int i=tickh; (i<h && i<(128*tickh)); i+=tickh)
    {
        this->scene->addLine(0,i,w,i);
    }

    this->cursorItem = this->scene->addLine(this->cursorPosition, 0, this->cursorPosition, h, QPen(Qt::red));
    int w2 = this->sceneRect().width();
    if (w2 > 10000000)
    {
        int a = 1;
    }
}

void MidiRoll::resizeEvent(QResizeEvent *event)
{
    this->updateEvents();
}

void MidiRoll::setup()
{
    this->scene = new QGraphicsScene();
    this->updateEvents();
    this->setScene(this->scene);
    this->show();
}
