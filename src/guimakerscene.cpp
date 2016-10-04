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

        QByteArray pieceData = event->mimeData()->data("image/x-gui-element");
                QDataStream stream(&pieceData, QIODevice::ReadOnly);

//                QPoint offset;
                QPixmap pixmap;
                QString textData = event->mimeData()->data("text/plain");



                MainWindow *pMainWindow = MainWindow::getInstance();
                if (!pMainWindow) {return;}

//                create blank pixmap, for drawing text on it
                    if (!pMainWindow->getCurrLangStrategy()->recreatePixmap(textData, pixmap))
                        stream >> pixmap ;

//                    qDebug() << " " << pixmap.size() ;

//                if (textData == "StaticText"
//                        || textData == "Button")
//                {
//                    pixmap = QPixmap(128, 36);
//                    pixmap.fill(Qt::white);
//                }
//                else if (textData == "CompositeView")
//                {
//                    pixmap = QPixmap(192, 128);
//                    pixmap.fill(Qt::gray);
//                }
//                else
//                    stream >> pixmap /*>> offset*/;

                    int x = event->scenePos().x();
                    int y = event->scenePos().y();

    //                QRect square = QRect(QPoint(x, y), pixmap.rect().size());
                    QRect square = QRect(QPoint(0,0), pixmap.rect().size());

//                ---addItem

                    TreeItem *item =
                            pMainWindow->insertRowInAModels(event->mimeData()->data("text/plain"), square);

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
