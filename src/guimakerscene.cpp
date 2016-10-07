/*
 * Copyright (C) 2016 Tryson Hardie.
 *
 * This file is part of "GUI Maker".
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "guimakerscene.h"
#include "langstrategy.h"
#include "mainwindow.h"
#include "treeitem.h"

#include <QMimeData>
#include <QDebug>
#include <QDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

GuiMakerScene::GuiMakerScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

void GuiMakerScene::clear()
{
    qDeleteAll(items());
}


//---Drag and Drop
void GuiMakerScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
//    qDebug() << "scene" << "drag enter";
    event->setAccepted(true);
    //    QGraphicsScene::dragEnterEvent(event);
}

void GuiMakerScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
//    qDebug() << event->dropAction();
    event->setAccepted(true);

}

void GuiMakerScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{

    if (event->mimeData()->hasFormat("image/x-gui-element")) {
        MainWindow *pMainWindow = MainWindow::getInstance();
        if (!pMainWindow) {return;}

        QByteArray pieceData = event->mimeData()->data("image/x-gui-element");
                QDataStream stream(&pieceData, QIODevice::ReadOnly);

//                QPoint offset;
                QPixmap pixmap;
                QString nameOfelement = event->mimeData()->data("text/plain");


//                if AutoIt -> we need to change name of element
                pMainWindow->getCurrLangStrategy()->translateName(nameOfelement);

//                create blank pixmap, for drawing text on it
                    if (!pMainWindow->getCurrLangStrategy()->recreatePixmap(nameOfelement, pixmap))
                        stream >> pixmap ;

                    int x = event->scenePos().x();
                    int y = event->scenePos().y();

    //                QRect square = QRect(QPoint(x, y), pixmap.rect().size());
                    QRect square = QRect(QPoint(0,0), pixmap.rect().size());

//                ---addItem

//                    QString typeOfelement = event->mimeData()->data("text/plain");

                    TreeItem *item =
                            pMainWindow->insertRowInAModels(nameOfelement, square);

                    item->setPixmap(pixmap);
                    item->setPos(QPointF(x, y));
                    addItem(item);

                    //        ---Dropped over element?
                    item->isOverContainer();

                update(square);

            event->acceptProposedAction();

        }
        else {
            event->ignore();
    }
}

void GuiMakerScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();

    painter->fillRect(sceneRect, Qt::white);

    painter->setBrush(QColor(Qt::blue));
    painter->setPen(QColor(Qt::lightGray));


    // ---draw grid
    //    int step = 20;
    // vertical  --  x

    for (int x = 0; x < width(); x += 20) {
        painter->drawLine(x, 0, x, height());
//        painter->drawText(QPoint(x, 0), QString::number(x));

    }

    // horizontal -- y

    for (int y = 0; y < height(); y += 20) {
        painter->drawLine(0, y, width(), y);
//        painter->drawText(QPoint(0, y), QString::number(y));

    }
}

//code tools/

//get all the code
QString GuiMakerScene::getText(bool richText)
{

    QString resultText;

//    get code string of main window
    MainWindow *pMainWindow = MainWindow::getInstance();
    AbstractLangStrategy *lang = pMainWindow->getCurrLangStrategy();
    resultText = lang->get_Window(richText);

    if( !hasItems() )
        return resultText;


    //get code string of each element
    foreach (QGraphicsItem *item, items(Qt::AscendingOrder)) {
        if (TreeItem *element = qgraphicsitem_cast<TreeItem *>(item))
            resultText.append(lang->get_sourceCode(element, richText));

//            resultText.append(element->get_info(richText));
    }


    return resultText;
}







void GuiMakerScene::keyPressEvent(QKeyEvent* ke )
{

    //                moveBy(x,y);
    //                moveBy(qreal dx, qreal dy) -
    //                        equivalent to calling setPos(pos() + QPointF(dx, dy)).

    switch( ke->key() )
    {
    case Qt::Key_Up:
        foreach (QGraphicsItem *item, selectedItems( )) {
            item->moveBy(0, -quantization);
        }
        ke->accept();
        break;

    case Qt::Key_Down:
        foreach (QGraphicsItem *item, selectedItems( )) {
            item->moveBy( 0, +quantization );
        }
        ke->accept();
        break;

    case Qt::Key_Left:
        foreach (QGraphicsItem *item, selectedItems( )) {
            item->moveBy( -quantization, 0 );
        }
        ke->accept();
        break;

    case Qt::Key_Right:
        foreach (QGraphicsItem *item, selectedItems( )) {
            item->moveBy( +quantization, 0 );
        }
        ke->accept();
        break;

//    case Qt::Key_Delete:
//        //        deleteSelected();
//        ke->accept();
//        break;

    default:
        break;
    }

    update();


}

void GuiMakerScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
//                     event->buttonDownScenePos(Qt::LeftButton).y());
//    const QList<QGraphicsItem *> itemList = items(mousePos);
//    movingItem = itemList.isEmpty() ? 0 : itemList.first();

//    if (movingItem != 0 && event->button() == Qt::LeftButton) {
//        oldPos = movingItem->pos();
//    }

//    clearSelection();//Bug- multiply selection don't work

    if (event->button() == Qt::LeftButton) {
        foreach (QGraphicsItem *item, selectedItems()) {
            TreeItem *movingItem = qgraphicsitem_cast<TreeItem *>(item);

            if(movingItem)
            {
                movingItem->setOldPos(movingItem->pos());
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void GuiMakerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QVector<TreeItem *> itemList;

        foreach (QGraphicsItem *item, selectedItems()) {
            TreeItem *movingItem = qgraphicsitem_cast<TreeItem *>(item);

            if(movingItem && movingItem->oldPos() != movingItem->pos())
            {
//                movingItem->setOldPos(movingItem->pos());
                itemList.append(movingItem);
            }
        }

        if(!itemList.isEmpty())
            emit itemMoved(itemList);

        movingItem = 0;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

