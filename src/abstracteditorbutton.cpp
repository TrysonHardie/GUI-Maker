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


#include "abstracteditorbutton.h"

AbstractEditorButton::AbstractEditorButton(QWidget *parent): QToolButton(parent),
    m_parameter("")
{
    init();
}

void AbstractEditorButton::init()
{
    setFocusPolicy(Qt::NoFocus);
//    setText(tr("Changer"));
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

    connect(this, SIGNAL(clicked()), this, SLOT(showEditor()));
}

void AbstractEditorButton::setText(const QString &txt)
{
    m_parameter = txt;
}

QString AbstractEditorButton::getText()
{
    return m_parameter;
}
