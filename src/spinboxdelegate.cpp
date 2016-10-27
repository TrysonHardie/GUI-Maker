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


/*
    delegate.cpp

    A delegate that allows the user to change integer values from the model
    using a spin box widget.
*/

#include "arrayeditorbutton.h"
#include "arrayofcolorseditorbutton.h"
#include "fonteditorbutton.h"
#include "mainwindow.h"
#include "spinboxdelegate.h"
#include "tablemodel.h"

#include <QLineEdit>
#include <QDebug>
#include <QCheckBox>
#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex & index ) const
{
//    QWidget *editor = 0;

    //    which type of arguments? --is it a Color?
    MainWindow *pMainWindow = MainWindow::getInstance();
//    if (pMainWindow) {

        TableModel *tablemodel = pMainWindow->get_table_model();
        QString typeOf_argument = tablemodel->get_typeOf_argument(index);

        if(typeOf_argument.contains("Font"))
        {
            FontEditorButton *editor = new FontEditorButton(parent);

            connect(editor, &FontEditorButton::editingFinished,
                    this, &SpinBoxDelegate::commitAndCloseFontEditor);

            return editor ;
        }
        if(typeOf_argument.contains("Array"))
        {
            ArrayEditorButton *editor;

            if(typeOf_argument.contains("Colors"))
                editor = new ArrayEditorButton(parent, 1);
            else if(typeOf_argument.contains("stateArray"))
                editor = new ArrayEditorButton(parent, 3);
            else
                editor = new ArrayEditorButton(parent);

            connect(editor, &ArrayEditorButton::editingFinished,
                    this, &SpinBoxDelegate::commitAndCloseArrayEditor);

            return editor ;
        }
        //        color in the format "#RRGGBB": "#00ffff"
        if(typeOf_argument.contains("Color"))
        {
            ArrayOfColorsEditorButton *editor = new ArrayOfColorsEditorButton(parent);
            connect(editor, &ArrayOfColorsEditorButton::editingFinished,
                    this, &SpinBoxDelegate::commitAndCloseEditor);
            return editor ;
        }
        if(typeOf_argument.contains("Boolean"))
        {
            QCheckBox *editor = new QCheckBox(parent);
            return editor;
        }
        if(typeOf_argument.contains("Float") || typeOf_argument.contains("Integer")
                || typeOf_argument.contains("Number"))
        {
            QSpinBox *editor = new QSpinBox(parent);
            editor->setFrame(false);
//            editor->setMinimum(0);

            return editor;
        }



    //    ---its a String
    QLineEdit *editor = new QLineEdit(parent);
    return editor ;

}

void SpinBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    //read data from the model
    QString value = index.model()->data(index, Qt::EditRole).toString();

    //    which type of arguments? --is it a Color?
    MainWindow *pMainWindow = MainWindow::getInstance();
//    if (pMainWindow) {

        TableModel *tablemodel = pMainWindow->get_table_model();
        QString typeOf_argument = tablemodel->get_typeOf_argument(index);

//        writes it to the editor
        if(typeOf_argument.contains("Array"))
        {
            ArrayEditorButton *spinBox = static_cast<ArrayEditorButton*>(editor);
            spinBox->setText(value);
            return;
        }
        //"#00ffff"
        if(typeOf_argument.contains("Color"))
        {
            ArrayOfColorsEditorButton *spinBox = static_cast<ArrayOfColorsEditorButton*>(editor);
            spinBox->setText(value);
            return;
        }
        if(typeOf_argument.contains("Boolean"))
        {
            QCheckBox *spinBox = static_cast<QCheckBox*>(editor);

            if(value.contains("true"))
                spinBox->setChecked(true);
            else
                spinBox->setChecked(false);

            return;
        }
        if(typeOf_argument.contains("Float") || typeOf_argument.contains("Integer")
                 || typeOf_argument.contains("Number"))
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            spinBox->setValue(value.toInt());
            return;
        }
        if(typeOf_argument.contains("Font"))
        {
            FontEditorButton *spinBox = static_cast<FontEditorButton*>(editor);
            spinBox->setText(value);
            return;


        }


    //    ---its a String
    QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
    spinBox->setText(value);

}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    //    which type of arguments? --is it a Color?
    MainWindow *pMainWindow = MainWindow::getInstance();
//    if (pMainWindow) {

        TableModel *tablemodel = pMainWindow->get_table_model();
        QString typeOf_argument = tablemodel->get_typeOf_argument(index);

//        reads the contents of the editor and set it to the model
        if(typeOf_argument.contains("Array"))
        {
            ArrayEditorButton *spinBox = static_cast<ArrayEditorButton*>(editor);
            model->setData(index, spinBox->getText(), Qt::EditRole);
            return;
        }
        //    --its a Color
        if(typeOf_argument.contains("Color"))
        {
            ArrayOfColorsEditorButton *spinBox = static_cast<ArrayOfColorsEditorButton*>(editor);
            model->setData(index, spinBox->getText(), Qt::EditRole);
            return;
        }
        if(typeOf_argument.contains("Boolean"))
        {
            QCheckBox *spinBox = static_cast<QCheckBox*>(editor);
            model->setData(index, spinBox->isChecked(), Qt::EditRole);

            return;
        }
        if(typeOf_argument.contains("Float") || typeOf_argument.contains("Integer")
                || typeOf_argument.contains("Number"))
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            model->setData(index, spinBox->value(), Qt::EditRole);
            return;
        }
        if(typeOf_argument.contains("Font"))
        {
            FontEditorButton *spinBox = static_cast<FontEditorButton*>(editor);
            model->setData(index, spinBox->getText(), Qt::EditRole);
            return;
        }



    //    ---its a String
    QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
    model->setData(index, spinBox->text(), Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


void SpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
//    if (index.data().canConvert<ArrayOfColorsEditorButton>()) {
//        ArrayOfColorsEditorButton starRating = qvariant_cast<ArrayOfColorsEditorButton>(index.data());

//        if (option.state & QStyle::State_Selected)
//            painter->fillRect(option.rect, option.palette.highlight());

//        starRating.paint(painter, option.rect, option.palette,
//                          ArrayOfColorsEditorButton::ReadOnly);
//    } else {
        QStyledItemDelegate::paint(painter, option, index);
//    }
}

void SpinBoxDelegate::commitAndCloseEditor()
{
    ArrayOfColorsEditorButton *editor = qobject_cast<ArrayOfColorsEditorButton *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}


void SpinBoxDelegate::commitAndCloseArrayEditor()
{
    ArrayEditorButton *editor = qobject_cast<ArrayEditorButton *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void SpinBoxDelegate::commitAndCloseFontEditor()
{
    FontEditorButton *editor = qobject_cast<FontEditorButton *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
