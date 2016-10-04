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

#include "arrayeditor.h"
#include "arrayofcolorseditorbutton.h"
#include "coloreditorbutton.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
//#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>

#include <QDebug>

ArrayEditor::ArrayEditor(QWidget *parent, const QString &newtext, int typeOfArray) :
    AbstractArrayEditor(parent)
  ,m_typeOfArray(static_cast<ArraysTypes>(typeOfArray))

{
    items = newtext.split(",");
    //    add at least 4 rows
//    m_tableWidget = new QTableWidget(qMax(items.size(), 4), 1, this);
    m_tableWidget->setRowCount(qMax(items.size(), 4));
    m_tableWidget->setColumnCount(1);

//    set colorWidget in each row of tableWidget
    if(typeOfArrayIsColor())
    for (int index = 0; index < m_tableWidget->rowCount(); ++index) {
        m_tableWidget->setCellWidget(index, 0, new ArrayOfColorsEditorButton(this));
        }


    if(!newtext.isEmpty())
        parseText();

}


QString ArrayEditor::getText(QWidget *parent, const QString &newtext, int typeOfArray, bool *ok)
{
    ArrayEditor dlg(parent, newtext, typeOfArray);
//qDebug() << "typeOfArray " << typeOfArray ;
    const int result = dlg.exec();
    if (ok) *ok = result;

    return result == QDialog::Accepted ? dlg.getTxt() : "";

}

// set text to tableWidget from delegate
void ArrayEditor::parseText()
{


//    int rowcount = items.size();

    if(typeOfArrayIsColor())
        for (int row = 0; row < items.size(); ++row) {
            ArrayOfColorsEditorButton *currItem = static_cast<ArrayOfColorsEditorButton*>(m_tableWidget->cellWidget(row, 0));
            if(currItem )
                currItem->setText(items.at(row));
        }
    else
        for (int row = 0; row < items.size(); ++row) {
            QTableWidgetItem *newItem = new QTableWidgetItem(items.at(row));
            m_tableWidget->setItem(row, 0, newItem);
        }

}

bool ArrayEditor::typeOfArrayIsColor()
{
    return m_typeOfArray == ArraysTypes::Color;
}



//get text from this
QString ArrayEditor::getTxt()
{
//    prepareText();

    if(typeOfArrayIsColor())
        for (int i = 0; i < m_tableWidget->rowCount(); ++i) {
            ArrayOfColorsEditorButton *currItem = static_cast<ArrayOfColorsEditorButton*>(m_tableWidget->cellWidget(i, 0));
            if(currItem && !currItem->getText().isEmpty())
                m_txt.append(currItem->getText() + ",");
        }
    else
        for (int i = 0; i < m_tableWidget->rowCount(); ++i) {
            QTableWidgetItem *currItem = m_tableWidget->item(i, 0);
            if(currItem && !currItem->text().isEmpty())
                m_txt.append(currItem->text() + ",");
        }


//    remove last ","
    if(m_txt.size() > 2)
        m_txt.remove(m_txt.size()-1, 1);

    return m_txt;
}

void ArrayEditor::insertRow()
{
//    m_tableWidget->setRowCount(m_tableWidget->rowCount() + 1);
    m_tableWidget->insertRow(m_tableWidget->rowCount());

    if(typeOfArrayIsColor())
        m_tableWidget->setCellWidget(m_tableWidget->rowCount()-1, 0, new ArrayOfColorsEditorButton(this));

}




//shifting==-1 - move up, shifting==1 - move down
void ArrayEditor::moveRow(int shifting)
{
    QTableWidgetItem *currItem = m_tableWidget->currentItem();
    if(!currItem)
        return;

    int currRow = currItem->row();
    if(currRow + shifting >= m_tableWidget->rowCount() || currRow + shifting < 0)
        return;

    //        set string
    m_tableWidget->takeItem(currRow, 0);
    QTableWidgetItem *nextItem = m_tableWidget->takeItem(currRow+shifting, 0);

    m_tableWidget->setItem(currRow, 0, nextItem);
    m_tableWidget->setItem(currRow + shifting, 0, currItem);


    //    select
        m_tableWidget->setCurrentItem(currItem);

//    TODo- don't work for StatesEditor
//    specific method for derived classes
//    because StatesEditor needs to swap cellWidgets - ArrayOfColorsEditorButton
}

//QTableWidgetItem *currItem = m_tableWidget->currentItem();
//if(!currItem)
//    return;

//int currRow = m_tableWidget->currentRow();
//if(currRow + shifting >= m_tableWidget->rowCount() || currRow + shifting < 0)
//    return;

//m_tableWidget->takeItem(currRow, 0);
// //    remove row of current Item
//m_tableWidget->removeRow(currRow);

// //    insert row below current Item
//m_tableWidget->insertRow(currRow + shifting);
// //    set our Item in row below old position
//m_tableWidget->setItem(currRow + shifting, 0, currItem);
// //    set Item selected
//m_tableWidget->setCurrentItem(currItem);


AbstractArrayEditor::AbstractArrayEditor(QWidget *parent): QDialog(parent)
{
    m_tableWidget = new QTableWidget(this);

    seupWidgets();
}

void AbstractArrayEditor::seupWidgets()
{
    QPushButton *m_MoveUp = new QPushButton("MoveUp", this);
    QPushButton *m_addbutton = new QPushButton("AddElement", this);
    QPushButton *m_MoveDown = new QPushButton("MoveDown", this);


    QVBoxLayout *buttonLayout = new QVBoxLayout(this);
    buttonLayout->addWidget(m_MoveUp);
    buttonLayout->addWidget(m_addbutton);
    buttonLayout->addWidget(m_MoveDown);
//    buttonLayout->setSpacing(1);
//    buttonLayout->setStretchFactor(m_addbutton, 51);

    connect(m_MoveUp, SIGNAL(clicked(bool)), this , SLOT(moveRowUp()));
    connect(m_addbutton, SIGNAL(clicked(bool)), this , SLOT(insertRow()));
    connect(m_MoveDown, SIGNAL(clicked(bool)), this , SLOT(moveRowDown()));

    QWidget* buttonLayoutContainer = new QWidget(this);
    buttonLayoutContainer->setLayout(buttonLayout);

//    m_clearbutton = new QPushButton("Clear", this); m_tableWidget->clear()

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(m_tableWidget);
    splitter->addWidget(buttonLayoutContainer);
//    splitter->setSizes(QList<int>({128,32}));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(splitter);
    layout->addWidget(buttonBox);
}
