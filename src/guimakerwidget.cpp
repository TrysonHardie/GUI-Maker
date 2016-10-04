#include "diagramitem.h"
#include "guimakerwidget.h"

//#include <QDropEvent>
//#include <QMimeData>

//#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

GuiMakerWidget::GuiMakerWidget(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
//    myMode = MoveItem;
//    myItemType = DiagramItem::Step;
//    line = 0;
//    textItem = 0;
//    myItemColor = Qt::white;
//    myTextColor = Qt::black;
//    myLineColor = Qt::black;

//    setFocusPolicy( Qt::StrongFocus );

//    setFocus();
}

void GuiMakerWidget::clear()
{

}

//---Drag and Drop
void GuiMakerWidget::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "--dragEnterEvent ";
//    event->accept();
    QGraphicsScene::dragEnterEvent(event);
}
/*
void GuiMakerWidget::dropEvent(QGraphicsSceneDragDropEvent *event)
{
qDebug() << "--dropEvent ";

}
*/


void GuiMakerWidget::addNewItem(QGraphicsItem *item)
{
//    item = new QGraphicsRectItem(myItemType, myItemMenu);
//    item->setBrush(myItemColor);
    addItem(item);
//    item->setPos(mouseEvent->scenePos());
    //    emit iteg mInserted(item);
}

QString GuiMakerWidget::getText(bool richText)
{

    return QString();
}
