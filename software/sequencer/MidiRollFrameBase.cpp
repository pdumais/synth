#include "MidiRollFrameBase.h"
#include <QGraphicsRectItem>
#include <QMouseEvent>

MidiRollFrameBase::MidiRollFrameBase(QWidget *parent): QGraphicsView(parent)
{
    this->rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    this->rubberBand->hide();
    this->selectionGroup = 0;
    this->mode = MouseMode::Edit;

}

void MidiRollFrameBase::setup()
{
    this->initNewBlock();

    this->adding = new MidiRollState();
    this->selecting= new MidiRollState();
    this->idle = new MidiRollState();
    this->moving = new MidiRollState();
    this->state = idle;

    // Idle mouse handlers
    this->idle->onMousePressed = [=](QMouseEvent* event) {
        auto spos = this->mapToScene(event->pos());
        auto item = this->itemAt(event->pos());

        if (this->mode == MouseMode::Edit)
        {
            if (!item || dynamic_cast<QGraphicsItemGroup*>(item))
            {
                this->state = this->adding;
                this->updateNewBlockRect(spos);
                this->newBlock->setVisible(true);
            }
        }
        else if (this->mode == MouseMode::Select)
        {
            if ((event->modifiers() & Qt::ControlModifier) && item)
            {
                item->setSelected(!item->isSelected());
                this->updateSelection();
                // No change in state
            }
            else if (item && item->isSelected())
            {
                this->selectionGroup = this->scene()->createItemGroup(this->scene()->selectedItems());
                this->selectionGroup->setTransformOriginPoint(spos);
                this->setCursor(Qt::SizeAllCursor);
                this->state = this->moving;
            }
            else
            {
                this->state = this->selecting;
                this->rubberBand->setGeometry(QRect(event->pos(), QSize(10,10)));
                this->rubberBand->show();
            }
        }
   };
   this->idle->onMouseMove = [=](QMouseEvent* event) {
        auto item = this->itemAt(event->pos());

        if (item == this->sectionEndItem || item == this->sectionStartItem)
        {
            this->setCursor(Qt::SizeHorCursor);
        } else {
            this->setCursor(Qt::ArrowCursor);
        }

   };
   this->idle->onMouseReleased = [=](QMouseEvent* event) {
   };

    // "Adding" Mouse Event Handlers
    this->adding->onMousePressed = [=](QMouseEvent* event) {
    };
    this->adding->onMouseMove = [=](QMouseEvent* event) {
        auto spos = this->mapToScene(event->pos());
        this->updateNewBlockRect(spos);
    };
    this->adding->onMouseReleased = [=](QMouseEvent* event) {
        float ticks = this->newBlock->rect().x() - QUARTERNOTE_WIDTH; // Substract first column
        ticks = (ticks / (float)QUARTERNOTE_WIDTH)*this->ticksPerQuarterNote;

        this->handleNewBlockCreated(ticks, 127-(this->newBlock->rect().y()/QUARTERNOTE_HEIGHT));
        this->newBlock->setVisible(false);
        this->state = this->idle;
    };

    // "Selecting" Mouse Event Handlers
    this->selecting->onMousePressed = [=](QMouseEvent* event) {
    };
    this->selecting->onMouseMove = [=](QMouseEvent* event) {
        auto origin = this->rubberBand->pos();
        this->rubberBand->setGeometry(QRect(origin, event->pos()));
    };
    this->selecting->onMouseReleased = [=](QMouseEvent* event) {
        for (auto i : this->scene()->items())
        {
            i->setSelected(false);
        }
        QRect r = this->rubberBand->geometry();

        for (auto i : this->scene()->items(this->mapToScene(r), Qt::ItemSelectionMode::IntersectsItemBoundingRect))
        {
            i->setSelected(true);
        }

        this->updateSelection();
        this->rubberBand->hide();
        this->state = this->idle;
    };

    this->moving->onMouseMove = [=](QMouseEvent* event) {
        auto spos = this->mapToScene(event->pos());
        auto origin = this->selectionGroup->transformOriginPoint();
        auto diff = spos - origin;
        QTransform t;
        int y = diff.y() - ((int)diff.y() % QUARTERNOTE_HEIGHT);
        int x = diff.x();
        auto br = this->selectionGroup->boundingRect();
        if (br.x()+x < QUARTERNOTE_WIDTH) x = this->selectionGroup->transform().dx();
        if ((br.y()+br.height())+y >= (QUARTERNOTE_HEIGHT*128)) y = this->selectionGroup->transform().dy();
        if (br.y()+y < 0) y = this->selectionGroup->transform().dy();
        this->selectionGroup->setTransform(t.translate(x, y));
    };

    this->moving->onMouseReleased = [=](QMouseEvent* event) {
        std::vector<MidiRollEvent*> originals;
        std::vector<MidiRollEvent*> changed;

        int dy = this->selectionGroup->transform().dy()/QUARTERNOTE_HEIGHT;
        int dx = ((float)this->selectionGroup->transform().dx()/QUARTERNOTE_WIDTH)*this->ticksPerQuarterNote;
        for (auto item : this->selectionGroup->childItems())
        {
            MidiRollEvent* orig = item->data(2).value<MidiRollEvent*>();
            originals.push_back(new MidiRollEvent(*orig));
            MidiRollEvent* newEvent = new MidiRollEvent(*orig);
            newEvent->note -= dy;
            newEvent->start += dx;
            changed.push_back(newEvent);
        }

        this->selectionGroup->resetTransform();
        this->scene()->destroyItemGroup(this->selectionGroup);
        this->setCursor(Qt::ArrowCursor);
        this->state = this->idle;

        this->handleBlocksMoved(originals, changed);
        for (auto it : changed) delete it;
        for (auto it : originals) delete it;
    };


}

MidiRollFrameBase::~MidiRollFrameBase()
{
}

void MidiRollFrameBase::updateNewBlockRect(QPointF spos)
{
    int y = spos.y() - ((int)spos.y() % QUARTERNOTE_HEIGHT);
    int x = spos.x();
    if ( x < QUARTERNOTE_WIDTH) return; // Ignore movements on first row
    if (y >= (QUARTERNOTE_HEIGHT*128)) return;

    this->newBlock->setRect(x, y, QUARTERNOTE_WIDTH, QUARTERNOTE_HEIGHT);
}

void MidiRollFrameBase::initNewBlock()
{
    this->newBlock = new QGraphicsRectItem(0,0,0,0);
    this->newBlock->setBrush(Qt::red);
    this->newBlock->setOpacity(0.5);
    this->newBlock->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->scene()->addItem(this->newBlock);
    this->newBlock->setVisible(false);
}


void MidiRollFrameBase::mouseMoveEvent(QMouseEvent *event)
{
    auto spos = this->mapToScene(event->pos());
    this->state->onMouseMove(event);
    this->handleHover(128-(spos.y()/QUARTERNOTE_HEIGHT));
}

void MidiRollFrameBase::mousePressEvent(QMouseEvent *event)
{
    auto spos = this->mapToScene(event->pos());
    if ( spos.x() < QUARTERNOTE_WIDTH) return; // Ignore movements on first row

    this->state->onMousePressed(event);
}

void MidiRollFrameBase::mouseReleaseEvent(QMouseEvent *event)
{
    this->state->onMouseReleased(event);
}

void MidiRollFrameBase::setMouseMode(MouseMode mode)
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

void MidiRollFrameBase::updateSelection()
{
    for (auto item : this->scene()->items())
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

