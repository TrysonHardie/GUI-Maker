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

#ifndef FONTEDITORBUTTON_H
#define FONTEDITORBUTTON_H

#include "abstracteditorbutton.h"




class FontEditorButton : public AbstractEditorButton
{
    Q_OBJECT

public:
    FontEditorButton(QWidget *parent = 0);
public slots:
    void showEditor();
signals:
    void editingFinished();

};

#endif // FONTEDITORBUTTON_H
