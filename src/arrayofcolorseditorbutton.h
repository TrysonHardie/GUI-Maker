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

#ifndef ARRAYOFCOLORSEDITORBUTTON_H
#define ARRAYOFCOLORSEDITORBUTTON_H

#include <QWidget>

class QToolButton;

class ArrayOfColorsEditorButton : public QWidget
{
    Q_OBJECT
    QToolButton* m_button;

    QString m_parameter;
    void init();
public:
    explicit ArrayOfColorsEditorButton(QWidget *parent = 0);

    void setText(const QString &txt);
    QString getText();
signals:
    void editingFinished();

public slots:
    void showEditor();
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

};

#endif // ARRAYOFCOLORSEDITORBUTTON_H
