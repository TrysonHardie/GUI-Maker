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

#include "fonteditorbutton.h"

#include <QLabel>
#include <QToolButton>
#include <QFontDialog>
#include <QGridLayout>

FontEditorButton::FontEditorButton(QWidget *parent): AbstractEditorButton(parent)
{
//    init();
}

void FontEditorButton::showEditor()
{
    bool ok;

    QFont font = QFontDialog::getFont(
                    &ok, QFont("Helvetica [Cronyx]", 10), this);
    if (ok) {
        //        <font size="5" color="black" face="Arial">П</font>
        //Font("Helvetica", 12);
//        setText(QString("Font(\"%1\", %2)")
//                .arg(font.family()).arg(QString::number(font.pointSizeF())));

//        Font("Helvetica", 12); -> 12_Helvetica
        setText(QString("%2|%1")
                .arg(font.family()).arg(QString::number(font.pointSizeF())));
        emit editingFinished();


    }
//    else {
        // the user canceled the dialog; font is set to the initial
        // value, in this case Helvetica [Cronyx], 10
//    }

}

