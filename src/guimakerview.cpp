#include "guimakerview.h"

#include <QMimeData>
#include <QDebug>
#include <QDropEvent>

GuiMakerView::GuiMakerView(GuiMakerScene * scene) :
    QGraphicsView(scene)
{
//    setDragMode(QGraphicsView::RubberBandDrag);
    setAcceptDrops(true);
    setFocusPolicy( Qt::StrongFocus );

    setFocus();
}
//delete it
GuiMakerView::GuiMakerView(QGraphicsScene *scene) :
    QGraphicsView(scene)
{
    setAcceptDrops(true);
    setFocusPolicy( Qt::StrongFocus );

    setFocus();
}


void GuiMakerView::dragEnterEvent ( QDragEnterEvent * event )
{
//    qDebug() << event->answerRect() << "drag enter";
    event->accept();

    QGraphicsView::dragEnterEvent(event);
}

void GuiMakerView::dragMoveEvent(QDragMoveEvent *event)
{
//    qDebug() << event->dropAction();
    event->accept();

    QGraphicsView::dragMoveEvent(event);
}


void GuiMakerView::dropEvent ( QDropEvent * event )
{
//    qDebug() << "view" << "drop";

    QGraphicsView::dropEvent(event);
}


