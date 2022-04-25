#include "TrackWidget.h"
#include "ui_TrackWidget.h"
#include <QMouseEvent>

TrackWidget::TrackWidget(QWidget *parent) : QFrame(parent), ui(new Ui::TrackWidget)
{
    this->active = false;
    this->ui->setupUi(this);

}

void TrackWidget::setActive(bool active)
{
    this->active = active;

    QString values = active?"255,0,0":"128,0,0";
    this->ui->label_2->setStyleSheet("QLabel { background-color: rgba("+values+"); }");
}

void TrackWidget::setLabel(QString text)
{
    this->ui->label->setText(text);
}

void TrackWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->trackSelected(this);
    }
}

bool TrackWidget::isActive()
{
    return this->active;
}
