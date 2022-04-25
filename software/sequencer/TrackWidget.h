#ifndef TRACKWIDGET_H
#define TRACKWIDGET_H

#include <QWidget>
#include <QFrame>

QT_BEGIN_NAMESPACE
namespace Ui { class TrackWidget; }
QT_END_NAMESPACE

class TrackWidget : public QFrame
{
    Q_OBJECT
public:
    explicit TrackWidget(QWidget *parent = nullptr);

    void setLabel(QString text);
    void setActive(bool active);
    bool isActive();

protected:
    void mousePressEvent(QMouseEvent *event) override;


signals:
    void trackSelected(TrackWidget*);

private:
    bool active;
    Ui::TrackWidget *ui;

};

#endif // TRACKWIDGET_H
