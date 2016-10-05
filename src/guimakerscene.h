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

#ifndef GUIMAKERSCENE_H
#define GUIMAKERSCENE_H

#include <QGraphicsScene>

class TreeItem;

class GuiMakerScene : public QGraphicsScene
{
    Q_OBJECT

public:
//    GuiMakerScene(QMenu *itemMenu, QObject *parent);
    GuiMakerScene(QObject *parent);

    void clear();

    QString getText(bool richText = false);
protected:
    virtual void dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent ( QGraphicsSceneDragDropEvent * event );
//        void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void keyPressEvent(QKeyEvent *ke);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
signals:
      void itemSelected(QGraphicsItem *item);
      void itemMoved(TreeItem *movedItem, const QPointF &movedFromPosition);
      void itemMoved(QVector<TreeItem *> treeItems);

private:

    inline bool hasItems() const
    {
        return !items().isEmpty();
    }

    QGraphicsItem *movingItem;
    QPointF oldPos;

};


#endif // GUIMAKERSCENE_H
