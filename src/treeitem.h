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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QPixmap>
#include <QRect>
#include <QVariant>
#include <QDebug>
#include <QGraphicsRectItem>

const int quantization = 5;


class TreeItem : public QGraphicsRectItem
{
    inline void init();

    friend class SuperColliderLangStrategy;
    friend class AutoItLangStrategy;

public:
    TreeItem();
 //   TreeItem(TreeItem *parent=0 );
    //dummy root 1tem
    TreeItem(TreeItem *parent, bool root );

    TreeItem(TreeItem *parent, const QString &classname, const QString &varname);

    //copy constructor
    TreeItem(TreeItem *olditem);
    // { return copy(olditem); }
    // Assignment operator
//    TreeItem& operator=(const TreeItem& olditem)
//        { return copy(olditem); }
    // copy
//    TreeItem& copy(const TreeItem& olditem);

    ~TreeItem();



    TreeItem *child(int number) const;


    TreeItem *parent() const;
    int childCount() const;
    int childNumber();
    bool setData(int column, const QVariant &className, const QVariant &rect);
    bool insertChildren(int position, int count, int columns);
    int columnCount() const;



    bool setProperty(int column, int row, const QVariant &value);




//---getters

    QString get_info(bool richText = false) const;


    bool isContainerOfViews() const
    {
       return m_isContainerOfViews;
    }

    int get_methods_size() const
    {
        return methods.size();
    }

    QString get_methods() const
    {
        return methods.first().property;
    }

    // eg. key step
    QString methods_get_property(const int &index) const
    {
        if (0 <= index && index < methods.size())
            return methods.at(index).property;
        return QString();
    }
    // eg. 5
    QString methods_get_value(const int &index) const
    {
        if (0 <= index && index < methods.size())
            return methods.at(index).value;
        return QString();
    }
    // eg. A Number
    QString methods_get_typeOf_argument(const int &index) const
    {
        if (0 <= index && index < methods.size())
            return methods.at(index).typeOf_argument;
        return QString();
    }


    //---GuiElement
    QString className() const;

    inline const QPixmap & get_pixmap() const
    {
        return m_pixmap;
    }

    inline const QRect & rect() const
    {
        return m_rect;
    }

//    inline const QString & className() const
//    {
//        return m_className;
//    }
    inline const QString & varName() const
    {
        return m_varName;
    }



    //    ---setters

    void setPixmap(const QPixmap &newpixmap ) {
        m_pixmap = newpixmap;
        m_rect.size() = m_pixmap.size();
    }


    //    ---

    QString rect_of_element() const;


    bool setData(int column, const QVariant &className);

    bool removeChildren(int position, int count);

    void isOverContainer();

    void childItems2removeAt(int position);
    void childItems2append(int position, TreeItem *item);
    int indexOfChild(TreeItem *item);
    QPointF oldPos() const;
    void setOldPos(const QPointF &value);

signals:
    //    void itemdataChanged();
    //    void selectedChange(TreeItem *selectedItem);

public slots:
    void senddataChangedToTheModel();

    bool removeChildrenItem(int position, int count);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
//    void keyPressEvent(QKeyEvent *ke);
private:
//    TreeItem *parentTreeItem;
    enum Mods
    {
        NothingToDo, MoveTheElement,
        ResizeTheElement, SelectTheElement
    };

//    List of top-level childs, stored only by RootItem
    QList<TreeItem*> childItems2;
    bool itisaRootItem;

//    context menu
    QMenu *myContextMenu;

    QString m_typeName;
    QString m_className;
    QString m_varName;
    QRect m_rect; // ??
    QPixmap m_pixmap;

    bool m_isContainerOfViews;
    int m_current_toggle_num;

    Mods m_mode;

    QPointF myOldPos;



    void init_methods();

    void makeParent(TreeItem *newParent);
//    ---Methods - properties

    class Method {
    public:
        Method(QString prop, QString ty):
            property(prop),
            typeOf_argument(ty)
        { }

        /*const */QString property; // eg. key step
        QString value;              // eg. 5
        QString typeOf_argument;    // eg. A Number
    };

    QList<Method> methods;
    QString getProperty(const QString& propertyName) const;
    void parse_schelp_for_Methods(const QString &name);



    QString parentItemvarName() const;

    //    ---toggles for resize

//    if cursor over toggles for resize
    inline bool cursorAtToggle(const int &x, const int &y) const
    {
        return whichToggle(x,y);
    }

    inline int whichToggle(const int &x, const int &y) const
    {

        if(toggleRight().contains(x,y))
            return 3;
        else if(toggleRightBottom().contains(x,y) )
            return 2;
        else if(toggleBottom().contains(x,y))
            return 1;
        else
            return 0;
//            return false
    }



    inline QRect toggleRight() const
    {
        return QRect(m_rect.right()-4,
                     m_rect.center().y(),
                     7,7 );
    }

    inline QRect toggleRightBottom() const
    {
        return QRect(QPoint(m_rect.left() + m_rect.width() -5,
                            m_rect.top() + m_rect.height() - 5),
                     QSize(7,7) );
    }

    inline QRect toggleBottom() const
    {
        return QRect(QPoint(m_rect.left() + m_rect.width()/2 -1,
                            m_rect.bottom()-4),
                     QSize(7,7) );
    }




};


#endif // TREEITEM_H
