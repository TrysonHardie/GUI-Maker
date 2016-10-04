#include "puzzlewidget.h"

#include <QDrag>
#include <QDropEvent>
#include <QMimeData>
#include <QPainter>
//#include <QtWidgets>

PuzzleWidget::PuzzleWidget(int imageSize, QWidget *parent)
    : QWidget(parent), m_ImageSize(imageSize)
{
    setAcceptDrops(true);
    setMinimumSize(m_ImageSize, m_ImageSize);
    setMaximumSize(m_ImageSize, m_ImageSize);
}

void PuzzleWidget::clear()
{
    pieceLocations.clear();
    piecePixmaps.clear();
    pieceRects.clear();
    highlightedRect = QRect();
    inPlace = 0;
    update();
}

void PuzzleWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

void PuzzleWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void PuzzleWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QRect updateRect = highlightedRect.united(targetSquare(event->pos()));

    if (event->mimeData()->hasFormat("image/x-puzzle-piece")) {

        highlightedRect = targetSquare(event->pos());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        highlightedRect = QRect();
        event->ignore();
    }

    update(updateRect);
}

void PuzzleWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-puzzle-piece")) {

        QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
        QDataStream stream(&pieceData, QIODevice::ReadOnly);
//        QRect square = targetSquare(event->pos());
        QPixmap pixmap;
        QPoint location;
        stream >> pixmap >> location;

        QRect square = QRect(event->pos(), pixmap.rect().size());

        pieceLocations.append(location);
        piecePixmaps.append(pixmap);
        pieceRects.append(square);

        highlightedRect = QRect();
        update(square);

        event->setDropAction(Qt::MoveAction);
        event->accept();

}
}



void PuzzleWidget::mousePressEvent(QMouseEvent *event)
{
    QRect square = targetSquare(event->pos());
    int found = findPiece(square);

    if (found == -1)
        return;

    QPoint location = pieceLocations[found];
    QPixmap pixmap = piecePixmaps[found];
    pieceLocations.removeAt(found);
    piecePixmaps.removeAt(found);
    pieceRects.removeAt(found);

    if (location == QPoint(square.x()/pieceSize(), square.y()/pieceSize()))
        inPlace--;

    update(square);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << pixmap << location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("image/x-puzzle-piece", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - square.topLeft());
    drag->setPixmap(pixmap);

    if (drag->start(Qt::MoveAction) == 0) {
        pieceLocations.insert(found, location);
        piecePixmaps.insert(found, pixmap);
        pieceRects.insert(found, square);
        update(targetSquare(event->pos()));

        if (location == QPoint(square.x()/pieceSize(), square.y()/pieceSize()))
            inPlace++;
    }
}








int PuzzleWidget::findPiece(const QRect &pieceRect) const
{
    for (int i = 0; i < pieceRects.size(); ++i) {
        if (pieceRect == pieceRects[i])
            return i;
    }
    return -1;
}



















void PuzzleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::white);

//    if (highlightedRect.isValid()) {
//        painter.setBrush(QColor("#ffcccc"));
//        painter.setPen(Qt::NoPen);
//        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
//    }

    for (int i = 0; i < pieceRects.size(); ++i) {
        painter.drawPixmap(pieceRects[i], piecePixmaps[i]);
    }
    painter.end();
}

const QRect PuzzleWidget::targetSquare(const QPoint &position) const
{
    return QRect(position.x()/pieceSize() * pieceSize(), position.y()/pieceSize() * pieceSize(), pieceSize(), pieceSize());
}

int PuzzleWidget::pieceSize() const
{
    return m_ImageSize / 5;
}

int PuzzleWidget::imageSize() const
{
    return m_ImageSize;
}
