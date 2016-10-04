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

#include "mainwindow.h"
#include "treeitem.h"
#include "treemodel.h"
#include <QDebug>

TreeModel::TreeModel()
{
    rootItem = new TreeItem(0,true);
}


TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

int TreeModel::columnCount() const
{
    return 2;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Variable Name");
            case 1:
                return tr("Attributes");
            default:
                return QVariant();
        }
    }

    return QVariant();
}




QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();

}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}


QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);

    TreeItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

QModelIndex TreeModel::trueparentIndex(TreeItem *childItem) const
{

    TreeItem *parentItem = childItem->parent();
//    qDebug() << "childItem " << childItem;//

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}



QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    if (index.column() == 0)
        return item->varName();
    else if (index.column() == 1)
        return item->rect_of_element();

    return QVariant();

}

int TreeModel::rowCount(const QModelIndex &parent) const
{

    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();


}



bool TreeModel::setData(const QModelIndex &index, const QVariant &value, const QVariant &rect, int role)
{
    if (role != Qt::EditRole)
        return false;
    TreeItem *item = getItem(index);


    bool result = item->setData(index.column(), value, rect);

    if (result)
        emit dataChanged(index, index);

    return result;
}
//tree edit
bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);


    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;


    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}



bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);

    //    insertTOrootItem
    if (parentItem == rootItem)
    {
        success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    }
//    else
//        // never 1nvoked
//    {
// //        qDebug() << " " <<  parentItem;
// //         TreeItem *item = new TreeItem(parentItem);
//        new TreeItem();
//        success = true;
//    }

    endInsertRows();

    return success;
}

bool TreeModel::insertRow(int position, const QModelIndex &parent)
{
    return insertRows(position, 1, parent);
}


//make copy of item and  insertTOrootItem  if needed
TreeItem * TreeModel::insertCopyOfRow(TreeItem *item)
{
//    return insertRows(position, 1, parent);

    QModelIndex model_index = index(item->childNumber(), columnCount());

    TreeItem *parentItem = item->parent();
    TreeItem *newitem = 0;

    beginInsertRows(parent(model_index), item->childNumber()+1, item->childNumber()+1);
    newitem = new TreeItem(item);

    if (!parentItem)
    {
//        newitem = new TreeItem(item->parent(), item);

        int position = item->childNumber();
        if (position > -1)
        {
            rootItem->childItems2append(position,newitem);
        }

    }
//    else
//        newitem = new TreeItem(parentItem, item);




    endInsertRows();

    return newitem;
}






TreeItem *TreeModel::getrootItem() const
{
    if (rootItem)
    return rootItem;
    else
        return NULL;
}

//insert item to ChildItems2 array of rootItem
void TreeModel::insertTOrootItem(TreeItem *item)
{
    beginResetModel();
    int position = item->childNumber();
    if (position == -1)
        rootItem->childItems2append(rootItem->childCount(), item);
//        rootItem->getChildItems2().append(item);

    endResetModel();

}

//        remove from ChildItems2 array of rootItem.
void TreeModel::removeFROMrootItem(TreeItem *item)
{
    beginResetModel();

    int position = item->childNumber();

    if (position > -1 /* && position < rootItem->childCount()*/)
        rootItem->childItems2removeAt(position);
//    rootItem->getChildItems2().removeAt(position);

    endResetModel();

}



void TreeModel::someitemdataChanged(QModelIndex &index)
{
    emit dataChanged(index, index);
}

void TreeModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount(createIndex(0,0,rootItem)));
    delete rootItem;

    rootItem = new TreeItem(0,true);

    endRemoveRows();

}

//---bug and unused..
bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
//    TreeItem *parentItem = getItem(parent);
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);

//    delete Item
    if (parentItem == rootItem)
    {
        parentItem->removeChildren(position, 1);
//        delete rootItem->getChildItems2().takeAt(position);
    }
    else /*if(parentItem)*/
    {
        parentItem->removeChildrenItem(position, 1);
    }

    endRemoveRows();

    return success;
}

void TreeModel::removeRow(TreeItem *item)
{

    int position = item->childNumber();
    if(item->parent())
    {
        position += item->parent()->childNumber();
    }

    removeRows(position, 1, trueparentIndex(item));

}

