/*
 * Copyright (C) 2016 The Qt Company Ltd.
 * Contact: https://www.qt.io/licensing/
 * Modified work Copyright (C) 2016 Tryson Hardie.
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

#ifndef UNDOREDOCOMMANDS_H
#define UNDOREDOCOMMANDS_H


#include <QUndoCommand>

#include "treeitem.h"

class GuiMakerScene;

class MoveCommand : public QUndoCommand
{
public:
    enum { Id = 1234 };

//    MoveCommand(TreeItem *treeItem, const QPointF &oldPos,
//                QUndoCommand *parent = 0);

    MoveCommand(QVector <TreeItem *> treeItems,
                QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
//    bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    int id() const Q_DECL_OVERRIDE { return Id; }

private:
//    TreeItem *myTreeItem;
//    QPointF myOldPos;
//    QPointF newPos;

    QVector <TreeItem *> myTreeItems;
    QVector <QPointF> myOldPoses;
    QVector <QPointF> newPoses;
};



class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(GuiMakerScene *graphicsScene, QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    TreeItem *myTreeItem;
    GuiMakerScene *myGraphicsScene;
};



//class AddCommand : public QUndoCommand
//{
//public:
//    AddCommand(TreeItem::DiagramType addType, QGraphicsScene *graphicsScene,
//               QUndoCommand *parent = 0);
//    ~AddCommand();

//    void undo() Q_DECL_OVERRIDE;
//    void redo() Q_DECL_OVERRIDE;

//private:
//    TreeItem *myTreeItem;
//    QGraphicsScene *myGraphicsScene;
//    QPointF initialPosition;
//};


//QString createCommandString(TreeItem *item, const QPointF &point);


#endif // UNDOREDOCOMMANDS_H
