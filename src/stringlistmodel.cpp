#include "stringlistmodel.h"

#include <QDataStream>
#include <QMimeData>
#include <QStringListModel>
#include <QIODevice>
//StringListModel::StringListModel()
//{

//}

int StringListModel::rowCount(const QModelIndex &parent) const
{
    return stringList.count();
}

QVariant StringListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= stringList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return stringList.at(index.row());
    else
        return QVariant();
}


QVariant StringListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}





// ------drag
Qt::ItemFlags StringListModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return (QAbstractListModel::flags(index)|Qt::ItemIsDragEnabled);

    return Qt::ItemIsDropEnabled;
}



QStringList StringListModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

//QMimeData *StringListModel::mimeData(const QModelIndexList &indexes) const
//{
//    QMimeData *mimeData = new QMimeData();
//    QByteArray encodedData;

//    QDataStream stream(&encodedData, QIODevice::WriteOnly);

//    foreach (QModelIndex index, indexes) {
//        if (index.isValid()) {
//            QPixmap pixmap = qvariant_cast<QPixmap>(data(index, Qt::UserRole));
//            QPoint location = data(index, Qt::UserRole+1).toPoint();
//            stream << pixmap << location;
//        }
//    }

//    mimeData->setData("image/x-puzzle-piece", encodedData);
//    return mimeData;
//}




//Qt::ItemFlags StringListModel::flags(const QModelIndex &index) const
//{
// //    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);

//    if (index.isValid())
//        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
//    else
//        return Qt::ItemIsDropEnabled;
//}


//QStringList StringListModel::mimeTypes() const
//{
//    QStringList types;
//    types << "application/vnd.text.list";
//    return types;
//}

QMimeData *StringListModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }

    mimeData->setData("text/plain", encodedData);
    return mimeData;
}
