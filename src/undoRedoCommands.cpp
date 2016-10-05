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

#include "undoRedoCommands.h"
#include "treeitem.h"
#include "guimakerscene.h"

#include <QGraphicsScene>
//#include <QDebug>

//MoveCommand::MoveCommand(TreeItem *treeItem, const QPointF &oldPos,
//                 QUndoCommand *parent)
//    : QUndoCommand(parent)
//{
//    myTreeItem = treeItem;
//    newPos = treeItem->pos();
//    myOldPos = oldPos;
//}

MoveCommand::MoveCommand(QVector<TreeItem *> treeItems, QUndoCommand *parent)
    : QUndoCommand(parent),
      myTreeItems(treeItems)
{
//    myTreeItem = treeItems.first();
//    newPos = QPointF(120,120);
//    myOldPos = newPos;

    foreach (TreeItem *treeItem, myTreeItems) {
        newPoses.append(treeItem->pos());
        myOldPoses.append(treeItem->oldPos());
    }

}

//bool MoveCommand::mergeWith(const QUndoCommand *command)
//{

//    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
// //    TreeItem *item = moveCommand->myTreeItems;

//    if (myTreeItems != moveCommand->myTreeItems)
//    return false;

//    newPoses = moveCommand->newPoses;
// //    newPos = item->pos();
// //    setText(QObject::tr("Move %1")
// //        .arg(createCommandString(myTreeItem, newPos)));


//    return true;
//}



void MoveCommand::undo()
{
//    return;
    int j=0;

    foreach (TreeItem *treeItem, myTreeItems) {
//        QPointF oldpos = treeItem->oldPos();

//        treeItem->setOldPos(treeItem->pos());
        treeItem->setPos(myOldPoses.at(j++));
    }
    myTreeItems.first()->scene()->update();

    //    myTreeItem->setPos(myOldPos);
//    myTreeItem->scene()->update();
//    setText(QObject::tr("Move %1")
//        .arg(createCommandString(myTreeItem, newPos)));
}



void MoveCommand::redo()
{
//    return;

    int j=0;
    foreach (TreeItem *treeItem, myTreeItems) {
//        QPointF newpos = treeItem->oldPos();

//        treeItem->setOldPos(treeItem->pos());
        treeItem->setPos(newPoses.at(j++));
    }

//    myTreeItem->setPos(newPos);
//    setText(QObject::tr("Move %1")
//        .arg(createCommandString(myTreeItem, newPos)));
}



DeleteCommand::DeleteCommand(GuiMakerScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = scene;
    QList<QGraphicsItem *> list = myGraphicsScene->selectedItems();
    list.first()->setSelected(false);
    myTreeItem = static_cast<TreeItem *>(list.first());
//    setText(QObject::tr("Delete %1")
//        .arg(createCommandString(myTreeItem, myTreeItem->pos())));
}



void DeleteCommand::undo()
{
    myGraphicsScene->addItem(myTreeItem);
    myGraphicsScene->update();
}



void DeleteCommand::redo()
{
    myGraphicsScene->removeItem(myTreeItem);
}



//AddCommand::AddCommand(TreeItem::DiagramType addType,
//                       QGraphicsScene *scene, QUndoCommand *parent)
//    : QUndoCommand(parent)
//{
//    static int itemCount = 0;

//    myGraphicsScene = scene;
//    myTreeItem = new TreeItem(addType);
//    initialPosition = QPointF((itemCount * 15) % int(scene->width()),
//                              (itemCount * 15) % int(scene->height()));
//    scene->update();
//    ++itemCount;
//    setText(QObject::tr("Add %1")
//        .arg(createCommandString(myTreeItem, initialPosition)));
//}


//AddCommand::~AddCommand()
//{
//    if (!myTreeItem->scene())
//        delete myTreeItem;
//}


//void AddCommand::undo()
//{
//    myGraphicsScene->removeItem(myTreeItem);
//    myGraphicsScene->update();
//}



//void AddCommand::redo()
//{
//    myGraphicsScene->addItem(myTreeItem);
//    myTreeItem->setPos(initialPosition);
//    myGraphicsScene->clearSelection();
//    myGraphicsScene->update();
//}


//QString createCommandString(TreeItem *item, const QPointF &pos)
//{
//    return QObject::tr("%1 at (%2, %3)")
//        .arg(item->diagramType() == TreeItem::Box ? "Box" : "Triangle")
//        .arg(pos.x()).arg(pos.y());
//}

