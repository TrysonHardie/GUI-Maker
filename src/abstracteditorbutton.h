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



#ifndef ABSTRACTEDITORBUTTON_H
#define ABSTRACTEDITORBUTTON_H

#include <QToolButton>


//buttons for TableView of methods
class AbstractEditorButton : public QToolButton
{
    Q_OBJECT

public:
    AbstractEditorButton(QWidget *parent);
    void init();

public slots:
    virtual void showEditor() = 0;
    void setText(const QString& txt);
    QString getText();

protected:
    QString m_parameter;

};

#endif // ABSTRACTEDITORBUTTON_H
