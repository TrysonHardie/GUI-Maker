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
#include "arrayeditorbutton.h"
#include "stateseditor.h"

ArrayEditorButton::ArrayEditorButton(QWidget *parent, int typeOfArray): AbstractEditorButton(parent)
  ,m_typeOfArray(typeOfArray)
{

}

void ArrayEditorButton::showEditor()
{
    bool result;
    QString code;

//    3 - its a StatesEditor,         Color == 1, String == 2
    if(m_typeOfArray != 3)
        code = ArrayEditor::getText(this, getText(), m_typeOfArray, &result);
    else
        code = StatesEditor::getText(this, getText(), &result);

    if (result == QDialog::Accepted) {
        setText(code);
        emit editingFinished();

    }

}
