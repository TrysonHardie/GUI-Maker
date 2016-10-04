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

#ifndef PIECESMODEL_H
#define PIECESMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QPixmap>
//#include <QPoint>
#include <QStringList>

class QMimeData;

class PiecesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PiecesModel(int pieceSize, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent) Q_DECL_OVERRIDE;
    QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
    QStringList mimeTypes() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;

    void addPiece(const QPixmap &pixmap, const QString &nameOfElement);
    void addPieces(const QPixmap& pixmap);

    void clear();

private:
    QList<QPixmap> pixmaps;
    QList<QString> names;


    int m_PieceSize;
};

#endif // PIECESLIST_H
