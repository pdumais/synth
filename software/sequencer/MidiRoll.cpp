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
    this->adding = false;
    this->mode = MouseMode::Edit;
    this->setInteractive(true);

    this->setup();

}

void MidiRoll::setup()
{
    this->scene = new QGraphicsScene();
    this->updateEvents();
    this->setScene(this->scene);
    this->initNewBlock();
    this->show();
    this->rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    this->rubberBand->hide();
    this->setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);
}

void MidiRoll::initNewBlock()
{
    this->newBlock = new QGraphicsRectItem(0,0,0,0);
    this->newBlock->setBrush(Qt::red);
    this->newBlock->setOpacity(0.5);
    this->newBlock->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->scene->addItem(this->newBlock);
    this->newBlock->setVisible(false);
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
    if (events.size() != 0)
    {
        auto lastev = this->eventsList.last();
        this->scene->setSceneRect(0,0,((float)(lastev->start+lastev->length)/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH, 128*QUARTERNOTE_HEIGHT);
    }
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

    auto item = this->scene->addRect(x, y, w, h, QPen(Qt::black), col);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setData(1, col);
    item->setData(2, QVariant::fromValue(ev));

}

void MidiRoll::addEvent(MidiRollEvent* ev)
{
    this->eventsList.append(ev);
    this->createEventBlock(ev);
}



void MidiRoll::updateEvents()
{

    this->scene->clear();
    this->initNewBlock();
    this->scene->update();

    for (auto &ev : this->eventsList)
    {
        this->createEventBlock(ev);
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
    int x = QUARTERNOTE_WIDTH+(((float)tick/(float)this->ticksPerQuarterNote)*QUARTERNOTE_WIDTH);
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

    for (int i=0; i<=127; i++)
    {
        QString num = QString::number(127-i).rightJustified(3, '0');
        this->scene->addText(num)->setPos(1, (i*QUARTERNOTE_HEIGHT)-5);
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

void MidiRoll::updateNewBlockRect(QPointF spos)
{
    int y = spos.y() - ((int)spos.y() % QUARTERNOTE_HEIGHT);
    int x = spos.x();
    if ( x < QUARTERNOTE_WIDTH) return; // Ignore movements on first row
    if (y >= (QUARTERNOTE_HEIGHT*128)) return;

    this->newBlock->setRect(x, y, QUARTERNOTE_WIDTH, QUARTERNOTE_HEIGHT);
}

void MidiRoll::mouseMoveEvent(QMouseEvent *event)
{
    if (this->mode == MouseMode::Select)
    {
        auto origin = this->rubberBand->pos();
        this->rubberBand->setGeometry(QRect(origin, event->pos()));
        return;
    }

    auto spos = this->mapToScene(event->pos());
    if (this->adding) {
        //TODO: Snap to grid
        this->updateNewBlockRect(spos);
    }

    this->onRowHover(128-(spos.y()/QUARTERNOTE_HEIGHT));


}

void MidiRoll::mousePressEvent(QMouseEvent *event)
{
    auto spos = this->mapToScene(event->pos());
    if ( spos.x() < QUARTERNOTE_WIDTH) return; // Ignore movements on first row

    auto item = this->itemAt(event->pos());

    if (this->mode == MouseMode::Edit)
    {
        if (!item)
        {
            this->adding = true;
            this->updateNewBlockRect(spos);
            this->newBlock->setVisible(true);
        }
    }
    else if (this->mode == MouseMode::Select)
    {
        this->rubberBand->setGeometry(QRect(event->pos(), QSize(10,10)));
        this->rubberBand->show();
    }

}

void MidiRoll::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->mode == MouseMode::Select)
    {

        for (auto i : this->scene->items())
        {
            i->setSelected(false);
        }
        QRect r = this->rubberBand->geometry();

        for (auto i : this->scene->items(this->mapToScene(r), Qt::ItemSelectionMode::IntersectsItemBoundingRect))
        {
            i->setSelected(true);
        }

        this->updateSelection();
        this->rubberBand->hide();
        return;
    }

    if (!this->adding) return;

    this->adding = false;

    float ticks = this->newBlock->rect().x() - QUARTERNOTE_WIDTH; // Substract first column
    ticks = (ticks / (float)QUARTERNOTE_WIDTH)*this->ticksPerQuarterNote;

    this->onNewBlock(ticks, 127-(this->newBlock->rect().y()/QUARTERNOTE_HEIGHT), this->ticksPerQuarterNote);
    this->newBlock->setVisible(false);

}

void MidiRoll::updateSelection()
{
    for (auto item : this->scene->items())
    {
        QGraphicsRectItem* r = dynamic_cast<QGraphicsRectItem*>(item);
        if (!r) continue;

        if (r->isSelected())
        {
            r->setBrush(Qt::cyan);
        }
        else
        {
            r->setBrush(r->data(1).value<QColor>());
        }
    }
}

void MidiRoll::setMouseMode(MouseMode mode)
{
    this->mode = mode;

    if (mode == MouseMode::Edit)
    {
        this->setDragMode(QGraphicsView::NoDrag);
    }
    else if (mode == MouseMode::Select)
    {
        this->setDragMode(QGraphicsView::RubberBandDrag);
    }
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
