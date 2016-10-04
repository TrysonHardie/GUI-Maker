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

#ifndef ARRAYEDITOR_H
#define ARRAYEDITOR_H

#include <QDialog>


class QTableWidget;
//class QPushButton;

//base class for array editors - ArrayEditor and StatesEditor
class AbstractArrayEditor : public QDialog
{
    Q_OBJECT

protected:
    AbstractArrayEditor(QWidget *parent);

    void seupWidgets();

    QTableWidget *m_tableWidget;

    QString m_txt;
    QStringList items;

    virtual void parseText() = 0;
    virtual QString getTxt() = 0;

protected slots:

    virtual void insertRow() = 0;
    virtual void moveRow(int shifting) = 0;
    void moveRowDown()
    {
        moveRow(1);
    }
    void moveRowUp()
    {
        moveRow(-1);

    }

//    virtual void moveWatever(int shifting,int currRow) = 0;

};

class ArrayEditor : public AbstractArrayEditor
{
    Q_OBJECT

    ArrayEditor(QWidget *parent, const QString &newtext, int typeOfArray = 2);

    enum ArraysTypes{
        Color = 1,
        String = 2
    };

//    QPushButton *m_addbutton;
    void parseText();
    QString getTxt();

    bool typeOfArrayIsColor();
    ArraysTypes m_typeOfArray;

public:
    static QString getText(QWidget* parent, const QString &newtext, int typeOfArray = 2, bool *ok = 0);
protected slots:

    void insertRow();
    virtual void moveRow(int shifting);

};

#endif // ARRAYEDITOR_H
