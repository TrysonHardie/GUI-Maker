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

#include "tablemodel.h"
#include "treeitem.h"
#include <QColor>
#include <QDebug>

TableModel::TableModel()
{
        m_currentItem = 0;
//    m_currentItem = new TreeItem(0);

}

TableModel::~TableModel()
{
    if (m_currentItem)
        delete m_currentItem;
}

void TableModel::clear()
{
    beginResetModel();
//    beginRemoveRows(QModelIndex(), 0, rowCount());

//    dummy TreeItem
    m_currentItem = 0;

//    endRemoveRows();
    endResetModel();

}



QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    if (m_currentItem)
        return createIndex(row, column, m_currentItem);
    else
        return QModelIndex();
}

QModelIndex TableModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);

        return QModelIndex();
}


int TableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}


int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (m_currentItem)
    return m_currentItem->get_methods_size();

    return 0;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_currentItem)
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = m_currentItem;

    if (index.column() == 0)
        return item->methods_get_property(index.row());
    else if (index.column() == 1)
        return item->methods_get_value(index.row());


    return QVariant();
}




TreeItem *TableModel::getItem(const QModelIndex &index) const
{
    Q_UNUSED(index);

    if (m_currentItem)
    return m_currentItem;

    return 0;
}



void TableModel::setitem(TreeItem *item)
{
    beginResetModel();

    m_currentItem = item;
    endResetModel();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !m_currentItem)
        return false;

    bool result = m_currentItem->setProperty(index.column(), index.row(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

//    bool editable = m_currentItem->isEditable(index.row());

//    if (!editable || index.column() == 0)
//        return QAbstractItemModel::flags(index);

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Property");
            case 1:
                return tr("Value");
            default:
                return QVariant();
        }
    }

    return QVariant();
}


QString TableModel::get_typeOf_argument(const QModelIndex &index) const
{
    if (m_currentItem)
    return m_currentItem->methods_get_typeOf_argument(index.row());

    return QString();
}
