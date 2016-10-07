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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QItemSelectionModel>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QPixmap>

#include <QCloseEvent>

class PiecesModel;
class TreeModel;
class TableModel;
class TreeItem;

class QListView;
class GuiMakerScene;
class QTreeView;
class QTableView;
class QTextEdit;
class QComboBox;
class CentralSubWindow;

class AbstractLangStrategy;

class QUndoStack;

//class CentralSubWindow;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    friend class CentralSubWindow;
    friend class AbstractLangStrategy;
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow *getInstance();
    TreeItem *insertRowInAModels(const QString &str, const QRect &rect);

    enum CurrentLang{
        SuperCollider = 0,
        AutoIt
    };

    int getCurrentLang();
    QStringList listAllLangStrategies()const;
    AbstractLangStrategy *getCurrLangStrategy()const;

public slots:

    void treeModelsenddataChanged(TreeItem *item);

    TableModel *get_table_model();
    TreeModel *get_tree_model();

    QMenu *menuOfItem();

    int subWindow_width();
    int subWindow_height();

    void copyOfItemMake();

    QString subWindow_width_height() const;


    void shiftCopyOfItemMake();

    void itemMoved(TreeItem *movedItem, const QPointF &oldPosition);
    void itemMoved(QVector<TreeItem *> treeItems);

private slots:
    void loadImages();
    void reloadImages();

    void ReadtheSelectionState(const QItemSelection &selected, const QItemSelection &);
    void SettheSelectionState();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

    bool saveFile(QString &fileName);
    void on_actionCopyCode_triggered();

    void on_actionTest_triggered();

    void on_actionChangeLang_triggered();

    void resizeScene(const QSize &sze);

    int rowCount();


    void setCodeText(const int &pagenumber);
    QString sign();

    void realDeleteItem();
    void deleteItem();
//    void addItem();

    void about();
private:
    Ui::MainWindow *ui;

    // singleton.
    static MainWindow *m_pMainWindow;

    void setupWidgets();


    QListView *listOfElements;
    PiecesModel *model;

    GuiMakerScene *guiMakerscene; //    main view of this app;

    QTableView *tableview;

    TableModel *table_model;

    QTreeView *codeTree;
    TreeModel *tree_model;
    QItemSelectionModel *m_selectionModel;

//    QLabel *textWidget;
    QTextEdit *textWidget;
    QComboBox *m_pageComboBox;

//    CurrentLang m_currentLang;
    AbstractLangStrategy *m_currentLangStrategy;
    QVector<AbstractLangStrategy*> m_AllLangStrategies;

    QMenu *itemMenu;

    CentralSubWindow *subWindow1;

    QUndoStack *undoStack;
};



class CentralSubWindow : public QMdiSubWindow
{
    Q_OBJECT

signals:
    void winResized(QSize);
protected:
    void closeEvent(QCloseEvent *event)
    {
        MainWindow *pMainWindow = MainWindow::getInstance();
        if (pMainWindow) {
            pMainWindow->on_actionNew_triggered();
        }

        // closing is not allowed
            event->ignore();
    }

    void resizeEvent(QResizeEvent *resizeEvent)
    {
//        when have rezized, resize SceneRect too
        emit winResized(resizeEvent->size());
        QMdiSubWindow::resizeEvent(resizeEvent);
    }

};

#endif // MAINWINDOW_H
