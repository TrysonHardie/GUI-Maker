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

#include "mainwindow.h"
#include "settingsdialog.h"

#include <QGridLayout>
#include <QListWidget>
#include <qdialogbuttonbox.h>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{

//    list of programming languages
    m_supported_langs = new QListWidget;

    MainWindow *pMainWindow = MainWindow::getInstance();

    m_supported_langs->insertItems(0, pMainWindow->listAllLangStrategies( ));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);


    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_supported_langs);
    layout->addWidget(buttonBox);


//    if (pMainWindow) {
//    no good
        m_supported_langs->setCurrentRow(pMainWindow->getCurrentLang());
//        }

//        int currLangNumber = 0;
//        QString currLangName = pMainWindow->getCurrLangStrategy()->get_CurrentLangName();

//        foreach (QString strtg, pMainWindow->listAllLangStrategies( )) {
//            if(strtg == currLangName)
//                break;
//            ++currLangNumber;
//        };
//        m_supported_langs->setCurrentRow(currLangNumber);
}

int SettingsDialog::programming_lang()const
{
    return m_supported_langs->currentRow();
}
