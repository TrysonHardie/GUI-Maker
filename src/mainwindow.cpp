/*
 * Copyright (c) 2016 Tryson Hardie
 *
 * This file is part of "GUI Maker".
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#include "guimakerscene.h"
#include "mainwindow.h"
#include "piecesmodel.h"
#include "settingsdialog.h"
#include "spinboxdelegate.h"
#include "langstrategy.h"
#include "tablemodel.h"
#include "treeitem.h"
#include "treemodel.h"
#include "ui_mainwindow.h"

#include <QSplitter>
#include <QMessageBox>
#include <QListView>
#include <QTreeView>
#include <QTableView>
#include <QFile>
#include <QFileDialog>
#include <QStackedWidget>
#include <QComboBox>
#include <QLabel>
#include <qboxlayout.h>

#include <QClipboard>
#include <QDebug>
#include <QGraphicsView>
#include <QMdiArea>
#include <QTextEdit>

// ---Main window.


// singleton.
MainWindow *MainWindow::m_pMainWindow = NULL;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setWindowTitle("GUI Maker");

//    ToDo- setWindowIcon(QIcon()); //the icon of the executable application file itself, as it is presented on the desktop
//    platform-dependent

    // singleton setup.
    m_pMainWindow = this;


    setupWidgets();
//    must be initialized after GuiMakerScene
    m_AllLangStrategies.append(new SuperColliderLangStrategy());
    m_AllLangStrategies.append(new AutoItLangStrategy());

    m_currentLangStrategy = m_AllLangStrategies.first();

    loadImages();

    connect(m_selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(ReadtheSelectionState(QItemSelection,QItemSelection)));

    connect(guiMakerscene, SIGNAL(selectionChanged()),
                  this, SLOT(SettheSelectionState()));

//    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openDocument()));
//    connect(actionClose, SIGNAL(triggered()), this, SLOT(closeDocument()));
//    connect(actionNew, SIGNAL(triggered()), this, SLOT(newDocument()));
//    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveDocument()));
//    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));


}


MainWindow::~MainWindow()
{
    delete ui;

    // singleton shut-down.
    m_pMainWindow = NULL;
}


// singleton .
MainWindow *MainWindow::getInstance (void)
{
    return m_pMainWindow;
}


void MainWindow::loadImages()
{

//    loadImage("Button");
//    loadImage("ListView");
//    loadImage("StaticText");

//    loadImage("CheckBox");
//    loadImage("Knob");
//    loadImage("NumberBox");

//    loadImage("PopUpMenu");
//    loadImage("Slider");
//    loadImage("TextField");

//    //        CompositeView + StaticText
//    loadImage("CompositeView");


    QStringList fnames = {"Button", "ListView", "StaticText",
                        "CheckBox", "Knob", "NumberBox",
                        "PopUpMenu", "Slider", "TextField",
                        "CompositeView"
                        };

//    ToDo- here must be a Starategy
//    m_currentLangStrategy->loadPixmap(fileName);

    switch (getCurrentLang()) {

    case MainWindow::SuperCollider:

        foreach (QString fileName, fnames) {
            loadSuperCollider(fileName);
        }
        break;

    case MainWindow::AutoIt:

        foreach (QString fileName, fnames) {
            loadAutoIt(fileName);
        }
        break;

    default:
        break;
    }
}

void MainWindow::reloadImages()
{
    model->clear();
    loadImages();
}


void MainWindow::loadSuperCollider(QString &fileName) const
{
    QPixmap newImage;

//    load Image
    if (!newImage.load(":/data/img/GUIelements/" + fileName + ".png")) {
        return;
    }
    model->addPiece(newImage, fileName);
}

void MainWindow::loadAutoIt(QString fileName)
{
    QPixmap newImage;

//    load Image
    if (!newImage.load(":/data/img/GUIelements/" + fileName + ".png")) {
        return;
    }

    //    convert fileName
    if (fileName == "Button")
        fileName = "GUICtrlCreateButton";
    else if (fileName == "ListView")
        fileName = "GUICtrlCreateList";
    else if (fileName == "StaticText")
        fileName = "GUICtrlCreateLabel";

    else if (fileName == "CheckBox")
        fileName = "GUICtrlCreateCheckBox";
    else if (fileName == "Knob")
        return;
//        fileName = "";
    else if (fileName == "NumberBox")
        return;
//        fileName = "";

    else if (fileName == "PopUpMenu")
        return;
//        fileName = "GUICtrlCreateCombo";
    else if (fileName == "Slider")
        fileName = "GUICtrlCreateSlider";
    else if (fileName == "TextField")
        fileName = "GUICtrlCreateEdit";
    else return;
//    else if (fileName == "CompositeView")
//        fileName = "";


    model->addPiece(newImage, fileName);
}


void MainWindow::setupWidgets()
{

//    ---context menu
    QAction *deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from window"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    QAction *copyAction = new QAction(tr("Clone"), this);
    copyAction->setShortcut(tr("Clone"));
    copyAction->setStatusTip(tr("Clone item"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyOfItemMake()));


    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(copyAction);
    itemMenu->addSeparator();
    itemMenu->addAction(deleteAction);

    //          itemMenu->addAction(toFrontAction);
    //          itemMenu->addAction(sendBackAction);


    //    ---help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));


    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));




//    ---Scene
    guiMakerscene = new GuiMakerScene(this);
    QGraphicsView *centralGraphicsView = new QGraphicsView(guiMakerscene);


//    guiMakerscene* centralGraphicsView = new guiMakerscene(guiMakerscene);

//    QGraphicsView * mdiArea = new QGraphicsView(scene);



    //    ---QMdiArea
    QMdiArea *mdiArea = new QMdiArea;
        subWindow1 = new CentralSubWindow;
              subWindow1->setWidget(centralGraphicsView);
              subWindow1->resize(360, 360);
              mdiArea->addSubWindow(subWindow1);

              guiMakerscene->setSceneRect(QRectF(
                                             0, 0,
                                             subWindow1->width(), subWindow1->height()));

              connect(subWindow1, SIGNAL(winResized(QSize)), this, SLOT(resizeScene(QSize)));



    //    ---PiecesModel
    model = new PiecesModel(256, this);

    //    ---ListView
    listOfElements = new QListView();

    listOfElements->setViewMode(QListView::IconMode);
    listOfElements->setIconSize(QSize(96, 96));
    //        listOfElements->setGridSize(QSize(44, 44));
    listOfElements->setSpacing(5);
    //        listOfElements->setMovement(QListView::Snap);
    //        listOfElements->setAcceptDrops(true);

    listOfElements->setDragEnabled(true);
    listOfElements->setDropIndicatorShown(true);

    listOfElements->setModel(model);



    //        ---Tree Model
    tree_model = new TreeModel( );
    table_model = new TableModel();

    //    ---TreeView
    codeTree = new QTreeView();

    codeTree->setModel(tree_model);
    m_selectionModel = codeTree->selectionModel();


    //---TableView
    tableview = new QTableView;
    tableview->setModel(table_model);


    //---delegate
    SpinBoxDelegate *delegate = new SpinBoxDelegate(this);
    tableview->setItemDelegate(delegate);





    //---central Widget and tabs
//    textWidget = new QLabel(this);
    //    textWidget->setTextFormat(Qt::RichText);
    textWidget = new QTextEdit(this);
    textWidget->setReadOnly(true);
//    textWidget->setFontPointSize(2);
//    textWidget->setCurrentFont(  QFont("DejaVu Sans Mono", 14));



    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mdiArea);
    stackedWidget->addWidget(textWidget);

    m_pageComboBox = new QComboBox;
    m_pageComboBox->addItem(tr("GUI Elements"));
    m_pageComboBox->addItem(tr("Code of your programm"));


    //    ---central QSplitter
    QSplitter *layout = new QSplitter(this);
    layout->addWidget(m_pageComboBox);
    layout->addWidget(stackedWidget);
    layout->setOrientation(Qt::Vertical);
    layout->setSizes(QList<int>({8,256}));

    //    ---right QSplitter
    QSplitter *splitter2 = new QSplitter(this);
    splitter2->addWidget(codeTree);
    splitter2->addWidget(tableview);
    splitter2->setOrientation(Qt::Vertical);
    splitter2->setSizes(QList<int>({56,256}));

    //    ---left QSplitter
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(listOfElements);
    //        splitter->addWidget(stackedWidget);
    splitter->addWidget(layout);
    splitter->addWidget(splitter2);

    QList<int> listSizes = {64,256,128};
    //        listSizes << 128 << 128 << 128;
    splitter->setSizes(listSizes);

    setCentralWidget(splitter);


    //        ---connect

    connect(m_pageComboBox, SIGNAL(activated(int)),
            stackedWidget, SLOT(setCurrentIndex(int)));
    connect(stackedWidget, SIGNAL(currentChanged(int)),
            this, SLOT(setCodeText(int)));

}


//    no good?
int MainWindow::getCurrentLang()
{
//    return static_cast<int>(m_currentLang);

    int currLangNumber = 0;
    QString currLangName = m_currentLangStrategy->get_CurrentLangName();

    foreach (AbstractLangStrategy* strtg, m_AllLangStrategies) {
        if(strtg->get_CurrentLangName() == currLangName)
            break;
        ++currLangNumber;
    };
    return currLangNumber;

//    This function requires  an implementation of operator==()
//    m_AllLangStrategies.indexOf(m_currentLangStrategy );

//    qDebug() << "m_currentLang " <<  m_currentLang;

}

QStringList MainWindow::listAllLangStrategies() const
{
    QStringList strtgNaems;
    foreach (AbstractLangStrategy* strtg, m_AllLangStrategies) {
        strtgNaems << strtg->get_CurrentLangName() ;
    };
    return strtgNaems;
}

AbstractLangStrategy *MainWindow::getCurrLangStrategy() const
{
    return m_currentLangStrategy;
}




//---Tree Model

TreeItem* MainWindow::insertRowInAModels(const QString &str, const QRect &rect)
{
//    return NULL;

    TreeModel *model = tree_model;
    QModelIndex index =
            model->index(model->rowCount(), model->columnCount() );

    if (!model->insertRow(index.row()+1, index.parent()))
        return NULL;

    QModelIndex child = index;
    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        child = model->index(index.row()+1, column, index.parent());
        model->setData(child, QVariant(str), QVariant(rect), Qt::EditRole);
    }

      TreeItem *item = model->getItem(child);

    if(item)
        table_model->setitem(item);

    return item;
}


//sends dataChanged signal to tree_model, when TreeItem state has change
void MainWindow::treeModelsenddataChanged(TreeItem *item)
{
//    return;

    TreeModel *model = tree_model;
    QModelIndex index = model->index(item->childNumber(), model->columnCount() );


    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row(), column, index.parent());
        model->dataChanged(child, child);
    }

}




//when item in tree selected
void MainWindow::ReadtheSelectionState(const QItemSelection &selected,
 const QItemSelection &)
{
//return;
    TreeModel *model = tree_model;

//    QModelIndex index;
//    QModelIndexList items = selected.indexes();

//    QModelIndex child;

//    foreach (index, items) {
// //        QString text = QString("(%1)").arg(index.row());
// //        model->setData(index, text);
// //        index.row()
// //        qDebug() << text;
//        child = model->index(index.row(), 0, index.parent());
//    }


    QModelIndex index = selected.indexes().value(0);

    QModelIndex child = model->index(index.row(), 0, index.parent());
//    set this item selected if not
//    if already selected
//    if(model->getItem(child) == guiMakerscene->selectedItems().first())
//        return;
//    if (scene->selectedItems().isEmpty())
//              return;


    //if already selected
    if(model->getItem(child)->isSelected())
        return;

//    else - deselect others and setSelected
    guiMakerscene->clearSelection();
    model->getItem(child)->setSelected(true);

    table_model->setitem(model->getItem(child));
}



//bug- item in treeView not selected clearly
//when item in GuiMakerWidget selected
void MainWindow::SettheSelectionState()
{
//return;
    if(!guiMakerscene->selectedItems().isEmpty())
    {
        TreeItem *item = qgraphicsitem_cast<TreeItem *>(guiMakerscene->selectedItems().last());
        table_model->setitem(item);
    }
    else
        return;

    TreeModel *model = tree_model;
//    m_selectionModel->clear();

    foreach (QGraphicsItem *gItem, guiMakerscene->selectedItems()) {

        TreeItem *item = qgraphicsitem_cast<TreeItem *>(gItem);

        //    retrieve the model indexes
        //            corresponding to the top-left and bottom-right items
        //            in the region to be selected
//        QModelIndex topLeft;
//        QModelIndex bottomRight;

        QModelIndex topLeft = model->index(item->childNumber(), 0, model->trueparentIndex(item));
        QModelIndex bottomRight = model->index(item->childNumber(), 1, model->trueparentIndex(item));

        if(m_selectionModel->isSelected(topLeft))
            continue;

        //    select this item in the model
        QItemSelection selection(topLeft, bottomRight);
        //m_selectionModel->select(selection, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            m_selectionModel->select(selection, QItemSelectionModel::Select );

    }

//    if(!guiMakerscene->selectedItems().isEmpty())
//    SettheSelectionState
//            (qgraphicsitem_cast<TreeItem *>(guiMakerscene->selectedItems().last()));
}

// set item selected  in table_model
//ToDo- only one item selected
//void MainWindow::SettheSelectionState(TreeItem *item)
//{
//    TreeModel *model = tree_model;

// //    QModelIndex index = model->index(item->childNumber(), model->columnCount() );

// //    qDebug() << index.isValid(); // true

// //    qDebug() << item; //
// //    qDebug() << item->childNumber(); // 3

// //    retrieve the model indexes
// //            corresponding to the top-left and bottom-right items
// //            in the region to be selected
//    QModelIndex topLeft;
//    QModelIndex bottomRight;

//    topLeft = model->index(item->childNumber(), 0, model->trueparentIndex(item));
//    bottomRight = model->index(item->childNumber(), 1, model->trueparentIndex(item));

// //    select these items in the model
//    QItemSelection selection(topLeft, bottomRight);
//    m_selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
// //    m_selectionModel->select(selection, QItemSelectionModel::Select);

//    table_model->setitem(item);


//}





//void MainWindow::removeRow(TreeItem *item)
//{
//    table_model->clear();
//    tree_model->removeRow(item);
//}

//--- Context Menu
void MainWindow::deleteItem()
{
//    QGraphicsItem *existedItem = 0;

    table_model->clear();

    foreach (QGraphicsItem *item, guiMakerscene->selectedItems()) {

        tree_model->removeRow(qgraphicsitem_cast<TreeItem *>(item));

//        existedItem = item->childItems().first();
//        delete childItems()
//        foreach (QGraphicsItem *itemc, item->childItems()) {
//            removeRow(qgraphicsitem_cast<TreeItem *>(itemc));
//        }

        //             remove item from tree
//        removeRow(qgraphicsitem_cast<TreeItem *>(item));
//        qDebug() << qgraphicsitem_cast<TreeItem *>(item)->rectStr() ;

        //erase item from scene - items should be no longer exist...
//        guiMakerscene->removeItem(item);


//        deleteThisItem(qgraphicsitem_cast<TreeItem *>(item));
    }

    //    if(qgraphicsitem_cast<TreeItem *>(existedItem))
    //        qDebug() << qgraphicsitem_cast<TreeItem *>(existedItem)->rectStr() ;

}


//---create new Item: copy of selected
void MainWindow::copyOfItemMake()
{

    foreach (QGraphicsItem *gitem, guiMakerscene->selectedItems()) {

        TreeItem *item = qgraphicsitem_cast<TreeItem *>(gitem);
        //---create new Item: copy of selected one
//        TreeItem *newitem = new TreeItem(item->parent(), item);

        //        insert Copy in the model
        TreeModel *model = tree_model;

//        QModelIndex index =
//                model->index(model->rowCount(), model->columnCount() );

//        TreeItem *newitem = model->insertCopyOfRow(index.row()+1, index.parent(), item);


//        bug
        TreeItem *newitem = model->insertCopyOfRow(item);

        //      add to the scene
        if(!newitem->scene())
                item->scene()->addItem(newitem);

//        newitem->setParentItem(item->parentItem());

//        newitem->setPos(item->x()+15, item->y()+15);
        newitem->setPos(item->x()+25, item->y()+25);

//        this->cursor().setPos(mapToLocal(QPoint(item->x()+25, item->y()+25)));

//        QCursor c = cursor();
//        c.setPos(mapToGlobal(QPoint(item->x()+25, item->y()+25)));
//        setCursor(c);

        //        guiMakerscene->addItem(newitem);
        item->setSelected(false);
        newitem->setSelected(true);




//        QModelIndex index =
//                tree_model->index(tree_model->rowCount(), tree_model->columnCount() );

        //        insert in the model
//        if faild remove our new copy...
//        if (!tree_model->insertCopyOfRow(index.row()+1, newitem, index.parent()))
//        {
//            item->parent()->removeChildren(newitem->childNumber(), 1);
//            return;
//        }


//QGraphicsScene::addItem: item has already been added to this scene
//      add to the scene
//        item->scene()->addItem(newitem);
//        guiMakerscene->addItem(newitem);

    }
}

//---when press Shift: shift positions of origin items and create copy of selected ones
void MainWindow::shiftCopyOfItemMake()
{
//TreeItem *clickedItem
    foreach (QGraphicsItem *gitem, guiMakerscene->selectedItems()) {

        TreeItem *item = qgraphicsitem_cast<TreeItem *>(gitem);
        //        insert Copy in the model
        TreeModel *model = tree_model;

//        bug
        TreeItem *newitem = model->insertCopyOfRow(item);

        //      add to the scene
        if(!newitem->scene())
                item->scene()->addItem(newitem);

        newitem->setParentItem(item->parentItem());
        newitem->setPos(item->x(), item->y());


//        if(item == clickedItem)
//        shift positions of origin item
            item->setPos(item->x()+25, item->y()+25);
//        else
//        {
//            newitem->setPos(item->x()+25, item->y()+25);
//            item->setSelected(false);
//            newitem->setSelected(true);
//        }



    }
}

//used by tree model to count items in the scene  with no parent
int MainWindow::rowCount()
{

    //    every item with no parent its like a child of root1tem
//        QList<QGraphicsItem *> rootItems;

    int noparentitemsCount;
    foreach (QGraphicsItem *item, guiMakerscene->items()) {
        if(!item->parentItem())
            ++noparentitemsCount;
        //                rootItems.append(item);
    }

    return noparentitemsCount;
//        return rootItems.count();
//        return guiMakerscene->items().count();
}


TableModel *MainWindow::get_table_model()
{
    return table_model;
}

TreeModel *MainWindow::get_tree_model()
{
//    return NULL;

    return tree_model;
}











//Save as
// ToDo- just uncomment it
void MainWindow::on_actionSave_triggered()
{
//    qDebug() << sign() + guiMakerscene->getText();

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return;
    saveFile(dialog.selectedFiles().first());
}

bool MainWindow::saveFile(QString &fileName)
{
    QString extention = m_currentLangStrategy->get_extention();

//    switch (getCurrentLang()) {
//    case MainWindow::SuperCollider:
//        extention = ".scd";
//        break;
//    case MainWindow::AutoIt:
//        extention = ".au3";
//        break;
//    default:
//        break;
//    }

    if (fileName.right(4) != extention)
        fileName+=extention;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << sign() + guiMakerscene->getText();
//           textEdit->toPlainText();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}


//    clear all models
void MainWindow::on_actionNew_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Clear and start again"),
                                    tr("Are you sure about that?"),
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes)
    {
        table_model->clear();
        tree_model->clear();
        guiMakerscene->clear();

    }
}


//    set generated Code to the TextView
void MainWindow::setCodeText(const int &pagenumber)
{
//    TextView plased on Page 2
    if (pagenumber != 1)
        return;
//getRichText
    textWidget->setHtml(QString("<font size=\"4\">%1</font>")
                        .arg(guiMakerscene->getText(true)));

//    textWidget->setHtml(guiMakerscene->getText(true));

}

QMenu * MainWindow::menuOfItem()
{
    return itemMenu;
}

//get size of Window
int MainWindow::subWindow_width()
{
    return subWindow1->width();
}

int MainWindow::subWindow_height()
{
    return subWindow1->height();
}

//get width and height of Window, eg. - "360, 360"
QString MainWindow::subWindow_width_height() const
{
    return QString(QString::number(subWindow1->width( ))+ ", "+
                   QString::number(subWindow1->height( )));
}

//        when CentralSubWindow have resized, resize SceneRect too
void MainWindow::resizeScene(const QSize &sze)
{
    guiMakerscene->setSceneRect(QRectF( QPointF(0,0), sze));
//    guiMakerscene->sceneRect().setSize(sze);
//    qDebug() << "resizeScene ";
}



//Copy the code
void MainWindow::on_actionCopyCode_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(sign() + guiMakerscene->getText());

    statusBar()->showMessage(tr("All the code copied"), 2000);

//    show the code, that was copied
    m_pageComboBox->setCurrentIndex(1);
    m_pageComboBox->activated(1);

}

//    add commented line "/*signature*/" at the beginning of the files
QString MainWindow::sign()
{
    QString sgn = m_currentLangStrategy->get_commentedSignature();

    return sgn + "\n\n";
}









//tests
void MainWindow::on_actionTest_triggered()
{
}

//allow to change current programming language
void MainWindow::on_actionChangeLang_triggered()
{
    SettingsDialog* settingsdlg = new SettingsDialog(this);

    if (settingsdlg->exec() != QDialog::Accepted)
        return;
    else
    {
//        m_currentLang = static_cast<CurrentLang>(settingsdlg->programming_lang());
        m_currentLangStrategy = m_AllLangStrategies.value(settingsdlg->programming_lang( ));
        //    clear all models
        on_actionNew_triggered();
        //        reload all images according to new language (class names, etc.)
        reloadImages();
    }

}




void MainWindow::about()
{
//    (":/data/img/GUIelements/" + fileName + ".png");
    QString icon = ":/data/img/icons/gui_maker.png";

    QString sText = "<p><img src=\"" + icon + "\" alt=\"\" align=\"left\"></p>" + "<p>\n<big>";
    sText += "The <b>" GUI_MAKER_TITLE "</b> it is a free graphical app,<br/>";
    sText += "that helps to create GUI in some programming languages<br/>";
    sText += "like SuperCollider and AutoIt.<big><br/>\n";

    sText += tr("<b>Website</b>") + ": <a href=\"" GUI_MAKER_SITE "\">" GUI_MAKER_SITE "</a><br/>\n";
    sText += "<small><br />";
    sText += GUI_MAKER_COPYRIGHT "<br />\n";
    sText += "</small></p>\n";

   QMessageBox::about(this, tr("About ") + GUI_MAKER_TITLE, sText);
}
