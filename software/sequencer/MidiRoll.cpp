#include "MidiRoll.h"
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QRubberBand>
#include <QLabel>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QGuiApplication>


QColor cols[] = {Qt::red, Qt::blue, Qt::green, Qt::yellow};


MidiRoll::MidiRoll(QWidget *parent) : MidiRollFrameBase(parent)
{
    this->ticksPerQuarterNote = 192;
    this->track = 0;
    this->cursorPosition = 0;
    this->setInteractive(true);
    this->gridLines = 0;
    this->setup();

}

void MidiRoll::setup()
{
    this->scene = new QGraphicsScene();
    this->setScene(this->scene);
    MidiRollFrameBase::setup();
    this->updateEvents();
    this->show();
    this->setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);


}

QRect MidiRoll::getSection()
{
    QRect ret;
    float tickPerPixel = float(this->ticksPerQuarterNote)/float(QUARTERNOTE_WIDTH);

    float x = float(this->sectionStartItem->pos().x()-QUARTERNOTE_WIDTH)*tickPerPixel;
    float x2 = float(this->sectionEndItem->pos().x()-QUARTERNOTE_WIDTH)*tickPerPixel;
    ret.setX(int(x));
    ret.setWidth(int(x2-x));

    return ret;
}

void MidiRoll::updateEvent(MidiRollEvent* original, MidiRollEvent* updated)
{
    for (auto gitem : this->scene->items())
    {
        auto item = dynamic_cast<QGraphicsRectItem*>(gitem);
        if (!item) continue;

        MidiRollEvent* mre = item->data(2).value<MidiRollEvent*>();
        if (mre->note == original->note && mre->start == original->start && mre->channel == original->channel)
        {
            mre->note = updated->note;
            mre->start = updated->start;
            mre->length = updated->length;
            mre->channel = updated->channel;
            int x = ((float)mre->start/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH;
            int y = ((127-mre->note)*QUARTERNOTE_HEIGHT);
            int w = ((float)mre->length/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH;
            int h = (QUARTERNOTE_HEIGHT-1);
            x += QUARTERNOTE_WIDTH; // Skip 1st square because it's the number

            item->setRect(x, y, w, h);
            this->updateGridWidth();
            return;
        }
    }
}



void MidiRoll::setTicksPerQuarterNotes(int tpqn)
{
    this->ticksPerQuarterNote = tpqn;
}

void MidiRoll::setEvents(QVector<MidiRollEvent*>& events)
{

    // Delete old events
    for (auto ev : this->eventsList)
    {
        delete ev;
    }

    this->eventsList = events;

    float last = float(this->eventsList.last()->start+this->eventsList.last()->length);
    last = last / float(this->ticksPerQuarterNote);
    last = last * float(QUARTERNOTE_WIDTH);
    this->sectionStart = 0;
    this->sectionEnd = long(last);
    this->updateSection(this->sectionStart, this->sectionEnd);
    this->updateEvents();
}


void MidiRoll::createEventBlock(MidiRollEvent* ev)
{
    int x = ((float)ev->start/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH;
    int y = ((127-ev->note)*QUARTERNOTE_HEIGHT);
    int w = ((float)ev->length/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH;
    int h = (QUARTERNOTE_HEIGHT-1);

    x += QUARTERNOTE_WIDTH; // Skip 1st square because it's the number
    QColor col = QColor(cols[ev->channel]);
    QColor colborder = col.lighter(130);
    auto item = this->scene->addRect(x, y, w, h, QPen(colborder), col);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setData(1, col);
    item->setData(2, QVariant::fromValue(ev));
    item->setZValue(2);
}

void MidiRoll::addEvent(MidiRollEvent* ev)
{
    this->eventsList.append(ev);
    this->createEventBlock(ev);

    this->updateGridWidth();
}

void MidiRoll::updateGridWidth()
{
    long last = 0;
    for (auto &ev : this->eventsList)
    {
        long evend = ev->start+ev->length;
        if (evend > last) last = evend;
    }

    this->scene->setSceneRect(0,0,(QUARTERNOTE_WIDTH*20)+((float)(last)/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH, 128*QUARTERNOTE_HEIGHT);
    this->createGrid();
}


void MidiRoll::updateEvents()
{
    //TODO: dont clear the whole scene. just clear events
    this->scene->clear();
    this->gridLines = 0;
    this->initNewBlock();

    long last = 0;
    for (auto &ev : this->eventsList)
    {
        long evend = ev->start+ev->length;
        if (evend > last) last = evend;
        this->createEventBlock(ev);
    }
    this->scene->setSceneRect(0,0,(QUARTERNOTE_WIDTH*20)+((float)(last)/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH, 128*QUARTERNOTE_HEIGHT);

    this->createGrid();
    this->createSection();
    int h = this->sceneRect().height();
    this->cursorItem = this->scene->addLine(this->cursorPosition, 0, this->cursorPosition, h, QPen(Qt::red));
    this->scene->update();
}

void MidiRoll::setTrack(int track)
{
    this->track = track;
    this->updateEvents();
}

void MidiRoll::setCursorPosition(int tick)
{
    int h = this->sceneRect().height();
    int x = QUARTERNOTE_WIDTH+(((float)tick/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH);
    this->cursorPosition = x;

    this->cursorItem->setLine(x, 0, x, h-1);
}

void MidiRoll::createGrid()
{

    int tickw = QUARTERNOTE_WIDTH;
    int tickh = QUARTERNOTE_HEIGHT;
    int w = this->sceneRect().width()+tickw;
    int h = this->sceneRect().height();
    QList<QGraphicsItem*> list;

    if (this->gridLines)
    {
        for (auto c : this->gridLines->childItems()) delete c;
        delete this->gridLines;
        this->gridLines = 0;
    }
    // Each square is a quarternote
    for (int i=tickw; i<(w); i+=tickw)
    {
        list.append(this->scene->addLine(i,0,i,h-1));
    }

    for (int i=tickh; (i<h && i<(128*tickh)); i+=tickh)
    {
        list.append(this->scene->addLine(0,i,w,i));
    }

    for (int i=0; i<=127; i++)
    {
        QString num = QString::number(127-i).rightJustified(3, '0');
        auto t = this->scene->addText(num);
        t->setPos(1, (i*QUARTERNOTE_HEIGHT)-5);
        list.append(t);
    }

    this->gridLines = this->scene->createItemGroup(list);
}

void MidiRoll::createSection()
{
    int h = this->sceneRect().height();
    this->sectionStartItem = this->scene->addLine(0, 0, 0, h, QPen(Qt::blue,3));
    this->sectionEndItem = this->scene->addLine(0, 0, 0, h, QPen(Qt::blue, 3));
    this->updateSection(this->sectionStart, this->sectionEnd);
}

void MidiRoll::updateSection(int start, int end)
{
    this->sectionStartItem->setX(start+QUARTERNOTE_WIDTH);
    this->sectionEndItem->setX(end+QUARTERNOTE_WIDTH);
}

void MidiRoll::resizeEvent(QResizeEvent *event)
{
    this->updateEvents();
}

void MidiRoll::deleteSelection()
{
    for (auto i : this->scene->selectedItems())
    {
        MidiRollEvent* ev = i->data(2).value<MidiRollEvent*>();
        this->onBlockRemoved(ev);

        this->scene->removeItem(i);
        this->eventsList.removeOne(ev);
        delete ev;
        delete i;
    }
}

void MidiRoll::handleNewBlockCreated(long ticks, int note)
{
    this->onNewBlock(ticks, note , this->ticksPerQuarterNote);
}

void MidiRoll::handleBlocksMoved(std::vector<MidiRollEvent*> originals, std::vector<MidiRollEvent*> changed)
{
    this->onBlocksMoved(originals, changed);
}

void MidiRoll::handleHover(int note)
{
    this->onRowHover(note);
}
