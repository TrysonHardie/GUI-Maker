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

#include "arrayofcolorseditorbutton.h"

#include <QHBoxLayout>
#include <QColorDialog>
#include <QToolButton>
#include <QPainter>

ArrayOfColorsEditorButton::ArrayOfColorsEditorButton(QWidget *parent) : QWidget(parent)
{
    init();
}


void ArrayOfColorsEditorButton::init()
{
//    m_line = new QLineEdit(this);
    m_button =  new QToolButton(this);

    QHBoxLayout *mainLayout = new QHBoxLayout;

//    mainLayout->addWidget(m_line);
    mainLayout->addWidget(m_button);
    setLayout(mainLayout);

    setFocusPolicy(Qt::NoFocus);
//    setText(tr("Changer"));
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

    connect(m_button, SIGNAL(clicked()), this, SLOT(showEditor()));
}

void ArrayOfColorsEditorButton::showEditor()
{
    const QColor color = QColorDialog::getColor(QColor(getText()), this, "Select Color");

    if (color.isValid()) {
        setText(color.name());
        emit editingFinished();

    }
}

void ArrayOfColorsEditorButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
//    painter.begin(this);
//    QRectF rectangle(10.0, 20.0, 80.0, 60.0);

    QColor bgColor(m_parameter);
    if (bgColor.isValid())
        painter.fillRect(QRect(0, 0, width(), height()), bgColor);

//    painter.end();
}

void ArrayOfColorsEditorButton::setText(const QString &txt)
{
//    m_line->setText(txt);
    m_parameter = txt;
//    update();
}

QString ArrayOfColorsEditorButton::getText()
{
    return m_parameter;
}

