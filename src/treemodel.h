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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>


class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel();
    ~TreeModel();

    int columnCount(const QModelIndex &) const;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    TreeItem *getItem(const QModelIndex &index) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value, const QVariant &rect, int role);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    int columnCount() const;
    bool insertRows(int position, int rows, const QModelIndex &parent);
    bool removeRows(int position, int rows, const QModelIndex &parent);

    void removeRow(TreeItem *item);
    bool insertRow(int position, const QModelIndex &parent);

//    ---dummyRoot1tem
    TreeItem *getrootItem() const ;

    void insertTOrootItem(TreeItem *item);
    void removeFROMrootItem(TreeItem *item);
    TreeItem *insertCopyOfRow(TreeItem *item);
    QModelIndex trueparentIndex(TreeItem *childItem) const;
public slots:
    void someitemdataChanged(QModelIndex &index);
    void clear();

private:
    TreeItem *rootItem;

};

#endif // TREEMODEL_H
