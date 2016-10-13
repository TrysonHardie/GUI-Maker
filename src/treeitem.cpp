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


#include "mainwindow.h"
#include "treeitem.h"
#include "treemodel.h"
#include "guimakerwidget.h"
#include "langstrategy.h"

#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

// MVC and Graphics View Framework


//---Constructors

TreeItem::TreeItem() : QGraphicsRectItem(0)
    ,m_rect (QRect(0,0,60,90)),
      m_mode (NothingToDo),
      m_typeName ("")
{
    init();
}

//dummy root 1tem
//ToDo- maybe rootItem should be a separate class?
TreeItem::TreeItem(TreeItem *parent, bool root) : QGraphicsRectItem(parent)
    ,m_rect (QRect(0,0,60,90)),
      m_mode (NothingToDo),
      m_pixmap(QPixmap()),
      m_isContainerOfViews(false), // or true??
      itisaRootItem(root),
      m_current_toggle_num(0),
      myContextMenu(0)
{


}


TreeItem::TreeItem(TreeItem *parent, const QString &classname, const QString &varname)
    : QGraphicsRectItem(0)
    ,m_typeName (classname),
      m_varName (varname),
      m_rect (QRect(0,0,500,500)),
      m_mode (NothingToDo)
{
    Q_UNUSED(parent);

    init();
}

//copy constructor -- TreeItem *parent - is needed to avoid collision of constructors
TreeItem::TreeItem(TreeItem *olditem)
    : QGraphicsRectItem(olditem->parent()),
      m_typeName (olditem->m_typeName),
      m_className (olditem->className()),
      //      m_varName (olditem->varName()), // name of variable must be uniq
      m_rect (olditem->rect()),
      m_pixmap(olditem->get_pixmap()),
      m_isContainerOfViews(olditem->isContainerOfViews()),
      m_mode (NothingToDo),
      methods(olditem->methods)
{
    init();

    setZValue(olditem->zValue());

    if (m_className.contains("CompositeView"))
        //            random name of variable
    {
        srand (time(NULL));
        m_varName = "cntnr"+QString::number(rand()% 100 + rand()% 100);
    }

}

// copy
//TreeItem& TreeItem::copy (
//	const TreeItem& olditem )
//{
//	if (&olditem != this) {
// //        ...
//	}
//	return *this;
//}

TreeItem::~TreeItem()
{
    childItems2.clear();

    qDeleteAll(childItems());
}

void TreeItem::init()
{

    itisaRootItem = false;

    m_current_toggle_num = 0;

    MainWindow *pMainWindow = MainWindow::getInstance();
    if (pMainWindow) {
        myContextMenu = pMainWindow->menuOfItem();

        m_className = m_typeName;
                pMainWindow->getCurrLangStrategy()->translateName(m_className);

    }
    else
    {
        myContextMenu = 0;
        m_className = "";
    }

    setRect(m_rect);
          setFlag(QGraphicsItem::ItemIsMovable, true);
          setFlag(QGraphicsItem::ItemIsSelectable, true);
          setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
//          key events are only received for items that set the ItemIsFocusable flag
//          setFlag(QGraphicsItem::ItemIsFocusable, true);

          myOldPos = pos();
}



//---MVC part

TreeItem *TreeItem::child(int number) const
{

    if(itisaRootItem)
    {
        return childItems2.value(number);
    }
    else
        return qgraphicsitem_cast<TreeItem *>(childItems().value(number));

        //    every item with no parent its like a child of root1tem

}

TreeItem *TreeItem::parent() const
{
    TreeItem *parentEelement = qgraphicsitem_cast<TreeItem *>(parentItem());

//    if (parentEelement)
        return parentEelement;

//    return 0;

}


int TreeItem::childCount() const
{
    if(itisaRootItem)
        return childItems2.count();

    return childItems().count();
//
}

// Returns -1 if no item matched.
int TreeItem::childNumber() /*const*/
{
    if(itisaRootItem)
        return -1;

    if (parentItem())
    {
        return parentItem()->childItems().indexOf(this);
    }

    MainWindow *pMainWindow = MainWindow::getInstance();
    if (pMainWindow) {
        return pMainWindow->get_tree_model()->
                getrootItem()->indexOfChild(const_cast<TreeItem*>(this));
    }
//    qDebug() << "_";

    return 0;
}

//delete this rootItem's children
bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems2.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems2.takeAt(position);

    return true;
}

//delete this QGraphicsItem's children
bool TreeItem::removeChildrenItem(int position, int count)
{
    if (position < 0 || position + count > childItems().size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems().takeAt(position);

    return true;
}




//if this is Over ContainerView set new Parent
//ToDo- complicated?
void TreeItem::isOverContainer()
{

    if (scene()->collidingItems(this).size() > 0)
    {

        bool haveParent = false;
        //        test all items that collide with this item
        foreach (QGraphicsItem *item, scene()->collidingItems(this)) {
            if (TreeItem *element = qgraphicsitem_cast<TreeItem *>(item))
            {

                // if this element is over Container like CompositView -> makeParent
                if(element->isContainerOfViews())
                {
//                    if element already is a parent?
                    if(element != parentItem())
                        makeParent(element);
                    haveParent = true;
                    break;
                }
            }
        }
        if(!haveParent)
            makeParent(0);
    }
//    else restore Parent to root1tem
    else
        makeParent(0);
}



//for "MainWindow::insertRow"
bool TreeItem::setData(int column, const QVariant &className, const QVariant &rect)
{
    if (column < 0 || column >= 2)
        return false;

    if (column == 0)
    {
        m_typeName = className.toString();
        MainWindow *pMainWindow = MainWindow::getInstance();
        if (pMainWindow) {
            m_className = m_typeName;
                    pMainWindow->getCurrLangStrategy()->translateName(m_className);
        }

        init_methods();

//        ToDo- AutoIt support
//        m_className.contains("HLayoutView") ||
//        m_className.contains("VLayoutView")
        if (m_className.contains("CompositeView"))
        {
            m_isContainerOfViews = true;
            setZValue(-999);

//            random name of variable
            srand (time(NULL));
            m_varName = "cntnr"+QString::number(rand()% 100 + rand()% 100);
        }
        else
            m_isContainerOfViews = false;
    }
    else if (column == 1)
    {
        m_rect = rect.toRect();
        setRect(m_rect);
    }

    return true;
}

//handle TreeModels requests
bool TreeItem::setData(int column, const QVariant &className)
{
    if (column < 0 || column >= 2)
        return false;
    if (column == 0)
    {
        m_varName = className.toString();
    }
    return true;
}


//handle TableModels requests
bool TreeItem::setProperty(int column, int row, const QVariant &value)
{        
    if (column == 0)
    {
        return false;
    }
    else if (column == 1)
    {
        if (0 <= row && row < methods.size())
        {
            methods[row].value = value.toString();
        }
        return true;
    }
    else
        return false;


}


bool TreeItem::insertChildren(int position, int count, int columns)
{
    Q_UNUSED(columns);

    if (position < 0 || position > childItems2.size())
        return false;

    for (int row = 0; row < count; ++row) {
//        TreeItem *item = new TreeItem(this);
        TreeItem *item = new TreeItem();
        childItems2.insert(position, item);
    }

    return true;
}

int TreeItem::columnCount() const
{
    return 1;
}


//---acces to QList<TreeItem *> childItems2

//indexOf
int TreeItem::indexOfChild(TreeItem* item)
{
    return childItems2.indexOf(item);
}

//removeAt
void TreeItem::childItems2removeAt(int position)
{
    if (position < 0 || position> childItems2.size())
        return;

//    TreeItem *item = childItems2.takeAt(position);
                childItems2.removeAt(position);

}

//append
void TreeItem::childItems2append(int position, TreeItem* item)
{
        childItems2.insert(position, item);
}






//---Graphics View Framework  part

//selection Changes

QVariant TreeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if (change == ItemPositionChange && scene()) {
//        return value.toPointF();

        QRectF newPos(value.toPointF(), m_rect.size());
        QRectF rect = scene()->sceneRect();
        if (!rect.contains(newPos)) {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));

            int x = newPos.x();
            if(newPos.x() + m_rect.width() >= rect.right())
                x = (qMin(rect.right() - m_rect.width(), qMax(newPos.x(), rect.right())));

            int y = newPos.y();
            if(newPos.y() + m_rect.height() >= rect.bottom())
                y = (qMin(rect.bottom() - m_rect.height(), qMax(newPos.y(), rect.bottom())));
            return QPointF(x, y);
        }

        // if moved - quantize

        int x = newPos.x();
        int y = newPos.y();
        // quantize
        x = floor( x / quantization ) * quantization;
        y = floor( y / quantization ) * quantization;

        return QPointF(x, y);
    }
    return QGraphicsItem::itemChange(change, value);
}

QPointF TreeItem::oldPos() const
{
    return myOldPos;
}

void TreeItem::setOldPos(const QPointF &value)
{
    myOldPos = value;
}

//Executes contex menu for selected items
void TreeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //    scene()->clearSelection();
//    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

void TreeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
//    draw the Pixmap
    painter->drawPixmap(m_rect, m_pixmap);

//    draw toggles for resizing
    if(isSelected())
        painter->setBrush(QColor(Qt::cyan));
    else
        painter->setBrush(QColor(Qt::blue));

    painter->drawRect(toggleRight());
    painter->drawRect(toggleRightBottom());
    painter->drawRect(toggleBottom());

//    draw X coordinate at topLeft point?
//    painter->setPen(Qt::black);
//    painter->drawText(m_rect, QString::number((pos().x())));

    //    ---Draw Text
    painter->setFont(QFont("DejaVu Sans Mono", 14));

    MainWindow *pMainWindow = MainWindow::getInstance();
    pMainWindow->getCurrLangStrategy()->
            paintElement(this, painter);

}

void TreeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
//        if click on toggle
        if(whichToggle(event->pos().x(), event->pos().y()))
            m_mode = ResizeTheElement;
        else
        {
            m_mode = MoveTheElement;

//            myOldPos = pos();

            // copy
//            ToDo- move this to GuiMakerScene::mousePressEvent ?
            if( event->modifiers() & Qt::ShiftModifier )
            {
                MainWindow *pMainWindow = MainWindow::getInstance();
                pMainWindow->shiftCopyOfItemMake();

            }
        }


    }
    QGraphicsItem::mousePressEvent(event);

}

void TreeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if( m_mode == NothingToDo && event->buttons() == 0 )
    {
//            QApplication::setOverrideCursor(
//                    QCursor( Qt::SizeVerCursor ) );
            return;
    }

//    if( m_mode != NothingToDo )
//    {
        if( event->buttons() & Qt::LeftButton )
        {
            if( m_mode == MoveTheElement)
            {
                QGraphicsItem::mouseMoveEvent(event);

            }else
            if( m_mode == ResizeTheElement)
            {
                // resizem

                int x = event->pos().x();
                int y = event->pos().y();

                // quantizing
                x = floor( x / quantization ) * quantization;
                y = floor( y / quantization ) * quantization;

//                resizing
                if(x > 0 && y > 0)
                {
                    if (!m_current_toggle_num)
                    {
                        m_current_toggle_num = whichToggle(x, y);
                    }
                    // depending on number of toggle, change m_rect's parameters
                    if( m_current_toggle_num == 3 )
                    {
                        setCursor(Qt::SizeHorCursor);

                        m_rect.setWidth( x );
                    }
                    else if( m_current_toggle_num == 2 )
                    {
                        setCursor(Qt::SizeBDiagCursor);

                        m_rect.setSize( QSize(x, y) );
                    }
                    else if( m_current_toggle_num == 1 )
                    {
                        setCursor(Qt::SizeVerCursor);

                        m_rect.setHeight( y );
                    }
                }

            }

        }

        update();
}


void TreeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    If you want to change an item's geometry in any way,
//            you must first call prepareGeometryChange() to allow QGraphicsScene to update its bookkeeping.
    if(m_mode != NothingToDo)
    {
        prepareGeometryChange();
        setRect(m_rect);
    }


    if( event->button() & Qt::LeftButton && m_mode == MoveTheElement)
    {
        isOverContainer();
    }


    m_mode = NothingToDo;
    m_current_toggle_num = 0;

    setCursor(Qt::ArrowCursor);
//QApplication::restoreOverrideCursor();
    update();
    senddataChangedToTheModel();

    QGraphicsItem::mouseReleaseEvent(event);
}







//----mics

//get all the code
//        Qt's text widgets are able to display rich text: HTML 4 markup
QString TreeItem::get_info(bool richText) const
{
    QString result;

    MainWindow *pMainWindow = MainWindow::getInstance();
    AbstractLangStrategy *lang = pMainWindow->getCurrLangStrategy();

    result = lang->get_sourceCode(this, richText);

//    result = get_info_SuperCollider(richText);

    return result;
}









//---GuiElement
QString TreeItem::className()const
{
    return m_className;
}

//return string -    (w, Rect(20, 20, 340, 30));
QString TreeItem::rectStr()const
{
    return QString("(" + parentItemvarName() +", Rect(" + rect_of_element() + "));");
}


QString TreeItem::parentItemvarName() const
{
    TreeItem *parentEelement = qgraphicsitem_cast<TreeItem *>(parentItem());

    if (parentEelement)
    {
        return parentEelement->varName();
    }
    else
        return "w";
    //ToDo- possibility to change varName of main window. not "w" everytime
}

//get string representation of items rectangle - "20, 20, 340, 30"
QString TreeItem::rect_of_element() const
{
    int x = pos().x();
    int y = pos().y();
    return QString(QString::number(x) + "," + QString::number(y) + ", " +
                   QString::number(m_rect.width()) + "," + QString::number(m_rect.height()));

}

// whenever the data in an existing item changes.
void TreeItem::senddataChangedToTheModel()
{
    MainWindow *pMainWindow = MainWindow::getInstance();
    if (pMainWindow) {
        pMainWindow->treeModelsenddataChanged(this);
    }
}


void TreeItem::init_methods()
{
    //    append methods only once
        if (methods.isEmpty())
        {
//            special methods for this element
            parse_schelp_for_Methods(m_className);
//            methods for each element
            parse_schelp_for_Methods("CommonMethods");
        }
}

//Sets new parent to this item
void TreeItem::makeParent(TreeItem *newParent)
{
    MainWindow *pMainWindow = MainWindow::getInstance();
    TreeModel *model = pMainWindow->get_tree_model();

    //    if this element is over Container like CompositView
    if (newParent){
        this->setPos(scenePos().x() - newParent->pos().x(), scenePos().y() - newParent->pos().y());

        //        remove from rootItem.
        model->removeFROMrootItem(this);

    }
    //    if this element is over empty area
    else
    {
        this->setPos(scenePos().x(), scenePos().y());

        model->insertTOrootItem(this);

    }

    //    adds this graphics item to the scene of the parent - parent item will become a top-level item.
        this->setParentItem(newParent);
    //    If  0, this  will become a top-level item.

}




//parse .schelp files: finds Methods and Arguments
void TreeItem::parse_schelp_for_Methods(const QString &name)
{
    QString f;

    MainWindow *pMainWindow = MainWindow::getInstance();
    AbstractLangStrategy *lang = pMainWindow->getCurrLangStrategy();

    f = lang->get_pathToScFile(name);


//             qDebug() << "QString f " <<  f;
    //load(const QString &f);
    if (!QFile::exists(f))
    {
        return;
    }
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
    {
        return;
    }
    QString str = file.readAll();

    QString result, result_argument;


    //process_text(str);
    int i = 0, indexOf_method, indexOf_argument, indexOf_end;


    //METHOD
    //      EXAMPLES::

    indexOf_end = str.lastIndexOf("examples::", 0, Qt::CaseInsensitive);
    if(indexOf_end == -1)
    {
        indexOf_end = str.size()-1;
    }

//for each METHOD
    while(i < indexOf_end){
        result.clear(); result_argument.clear();

        //1. find ("method::")
        indexOf_method = str.indexOf("method::", i, Qt::CaseInsensitive)+9;
        if(indexOf_method == 8)
            break;

        for (i = indexOf_method; i < indexOf_end; ++i){
            if(str[i] == QChar('\n'))
                break;
            result.append(str.at(i));
        }

        if(i >= indexOf_end)
            break;


        //2. find ("argument::")
        indexOf_argument = str.indexOf("argument::", i, Qt::CaseInsensitive);
        indexOf_method = str.indexOf("method::", i, Qt::CaseInsensitive);

        //        no argument
        if(indexOf_argument == -1)
        {
//            methods.append(Method(result, ""));
            continue;
        }
        //        argument > next_method
        else if(indexOf_argument > indexOf_method && indexOf_method != -1)
        {
//            methods.append(Method(result, ""));
            continue;
        }

        //find alphabetic characters after "argument::": here can be '\n'
        indexOf_argument = str.indexOf(QRegularExpression("[a-zA-Z]"), indexOf_argument+10);

        for (i = indexOf_argument; i < indexOf_end; ++i){
            if(str[i] == QChar('\n'))
                break;
            result_argument.append(str.at(i));
        }
        methods.append(Method(result, result_argument));

        //    qDebug() << indexOf_argument;
        //    qDebug() << result;
        //    qDebug() << i;
    }


}













