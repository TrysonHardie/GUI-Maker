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

#include "stateseditor.h"
#include "arrayofcolorseditorbutton.h"
#include "coloreditorbutton.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QDebug>

//editor of button states - for SuperCollider

StatesEditor::StatesEditor(QWidget *parent, const QString &newtext) :
    AbstractArrayEditor(parent)

{

//    find all states - [str,Color#55ff00,Color#aaff7f]
    QRegExp stringRegEx("\\[([^\\]]*)\\]");
    int pos = 0;
    while ((pos = stringRegEx.indexIn(newtext, pos)) != -1) {
        items << stringRegEx.cap(1);
        pos += stringRegEx.matchedLength();
    }

//    add at least 4 rows
    m_tableWidget->setRowCount(qMax(items.size(), 4));
    m_tableWidget->setColumnCount(3);

//    Set the horizontal header labels
    QStringList sHorHeaderLabels;
    sHorHeaderLabels << "String" << "strColor" << "bgColor";
    m_tableWidget->setHorizontalHeaderLabels(sHorHeaderLabels);

    //    set CellWidgets- colorWidget in each 1,2 column of tableWidget
    for (int row = 0; row < m_tableWidget->rowCount(); ++row) {
        for (int column = 1; column < m_tableWidget->columnCount(); ++column) {
            m_tableWidget->setCellWidget(row, column, new ArrayOfColorsEditorButton(this));
        }
    }

    resize(512, 256);

    if(!newtext.isEmpty())
        parseText();
}

// set text to tableWidget from delegate
void StatesEditor::parseText()
{

//    split each state by the columns
    for (int row = 0; row < items.size(); ++row) {

        QString rowtext = items.at(row);
        qDebug() << rowtext;

//        ToDo- using QStringLists - How efficient is it?
        QStringList coloritems;
        QStringList stringitems;

//        QRegExp colorRegEx("(#[A-Za-z0-9]+)");
        QRegExp colorRegEx("Color.fromHexString\\(\"(#[A-Za-z0-9]+)");
        int pos = 0;
        while ((pos = colorRegEx.indexIn(rowtext, pos)) != -1) {
            coloritems << colorRegEx.cap(1);
            pos += colorRegEx.matchedLength();
        }
        // list: "#88ff88", "#11ff11"


//        QRegExp stringRegEx("\"([A-Za-z0-9]+)\"");
        QRegExp stringRegEx("\"([^\"]+)\""); //        everything but '\"'
        pos = 0;
        while ((pos = stringRegEx.indexIn(rowtext, pos)) != -1) {
            stringitems << stringRegEx.cap(1);
            pos += stringRegEx.matchedLength();
        }
        // list: "yes"


//        set string
        QTableWidgetItem *newItem = new QTableWidgetItem(stringitems.value(0));
        m_tableWidget->setItem(row, 0, newItem);
//        m_tableWidget->item(row, 0)->setText(stringitems.value(0));


//        set color;
        ArrayOfColorsEditorButton *currItem =
                static_cast<ArrayOfColorsEditorButton*>(m_tableWidget->cellWidget(row, 1));
        if(currItem )
            currItem->setText(coloritems.value(0));

        currItem = static_cast<ArrayOfColorsEditorButton*>(m_tableWidget->cellWidget(row, 2));
        if(currItem )
            currItem->setText(coloritems.value(1));
//        m_tableWidget->item(row, 1)->setText(coloritems.value(0));
//        m_tableWidget->item(row, 2)->setText(coloritems.value(1));

    }


}


QString StatesEditor::getText(QWidget *parent, const QString &newtext, bool *ok)
{
    StatesEditor dlg(parent, newtext);

    const int result = dlg.exec();
    if (ok) *ok = result;

    return result == QDialog::Accepted ? dlg.getTxt() : "";

}

//get text from this
QString StatesEditor::getTxt()
{
//    ["yes", #aa557f, #00ffff]
    for (int i = 0; i < m_tableWidget->rowCount(); ++i) {

        QString states("[");
        int filledColumns = 0;
//        m_txt.append("[");

        //get text - "txt",
        QTableWidgetItem *currItem = m_tableWidget->item(i, 0);
//        if(currItem && !currItem->text().isEmpty())
//        {
//            states.append("\"" + currItem->text() + "\",");
//            filledColumns++;
//        }
//        else/*         if(currItem)*/
//        {
//            states.append("\"\",");
//            filledColumns++;
//        }

        if(currItem)
        {
            states.append("\"" + currItem->text() + "\",");
            if(!currItem->text().isEmpty())
                filledColumns++;
        }

        //get color - Color.fromHexString(" ")
        for (int column = 1; column < m_tableWidget->columnCount(); ++column) {
            ArrayOfColorsEditorButton *currItem =
                    static_cast<ArrayOfColorsEditorButton*>(m_tableWidget->cellWidget(i, column));

            if (!currItem)
                break;

            if(!currItem->getText().isEmpty())
            {
                states.append("Color.fromHexString(\"" + currItem->getText() +  "\"),");
                filledColumns++;
            }
//            else if(filledColumns > 0)
//            {
//                if(column == 1)
//                    states.append("Color.fromHexString(\"#000000\"),"); // black for strColor
//                else
//                    states.append("Color.fromHexString(\"#ffffff\"),"); // white for bgColor

//                // previous columns already contains some data anyway
//                filledColumns++;
//            }

        }

        //    remove last ","
        if(filledColumns > 0)
        {
            //            m_txt.remove(m_txt.size()-1, 1);
            //            m_txt.append("],");
            states.replace(states.size()-1, 1, "],");
            m_txt.append(states);

        }
        //    remove first "[" or first "[\"\", "  whatever
//        else
//            states.clear();
//        states.remove(states.size()-1, 1);


    }

    //    remove last ","
        if(m_txt.size() > 2)
            m_txt.remove(m_txt.size()-1, 1);

        return m_txt;
}

void StatesEditor::insertRow()
{
    m_tableWidget->insertRow(m_tableWidget->rowCount());

    //    set CellWidgets- colorWidget in each 1,2 column of tableWidget
    for (int column = 1; column < m_tableWidget->columnCount(); ++column) {
        m_tableWidget->setCellWidget
                (m_tableWidget->rowCount()-1, column, new ArrayOfColorsEditorButton(this));
    }

}

void StatesEditor::moveRow(int shifting)
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

    //        set color;
    for (int column = 1; column < m_tableWidget->columnCount(); ++column) {

        ArrayOfColorsEditorButton *currColorsEditor =
                static_cast<ArrayOfColorsEditorButton*>(m_tableWidget->cellWidget(currRow, column));

        ArrayOfColorsEditorButton *currColorsEditor2 =
                static_cast<ArrayOfColorsEditorButton*>(m_tableWidget->cellWidget(currRow+shifting, column));

//        swap
        if(currColorsEditor && currColorsEditor2)
        {
            QString color1 = currColorsEditor->getText();

            currColorsEditor->setText(currColorsEditor2->getText());
            currColorsEditor2->setText(color1);

        }
    }
//    select
    m_tableWidget->setCurrentItem(currItem);
}

