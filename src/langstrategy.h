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

#ifndef LANGSTRATEGY_H
#define LANGSTRATEGY_H

#include <QString>

class MainWindow;
class GuiMakerScene;
class TreeItem;

class QPixmap;
class QPainter;

//---about
#define GUI_MAKER_SITE         "http://trysohard.info/GUI-Maker/"
#define GUI_MAKER_TITLE        "GUI Maker"
#define GUI_MAKER_COPYRIGHT    "Copyright (C) 2016 Tryson Hardie."

//---strategy design pattern

class AbstractLangStrategy
{
protected:
    MainWindow *m_pMainWindow;
    GuiMakerScene *m_pGuiMakerScene;

    QString m_langName;
    QString m_fileExtention;

public:
    AbstractLangStrategy(QString langName, QString fileExtention);

//    using by TreeItem
    virtual QString get_sourceCode(const TreeItem *treeItem, const bool &richText) const = 0;
    /*virtual*/ QString get_pathToScFile (const QString& name ) const;
    virtual void paintElement(const TreeItem *t, QPainter *painter) = 0;

    //    using by mainwindow
    /*virtual*/ QString get_extention ( ) const;
    virtual QString get_commentedSignature ( ) const = 0;
    //loadSuperCollider(fileName) = 0;

    //    using by Scene
    virtual QString get_Window (bool richText ) const = 0;
    virtual bool recreatePixmap (const QString& textData, QPixmap &pixmap ) const = 0;
    virtual void translateName (QString& nameOfElement) = 0;

    //    using by SettingsDialog
    /*virtual*/ QString get_CurrentLangName ( ) const;

//    QString get_ ( ) const = 0;
};

//---SuperCollider language
class SuperColliderLangStrategy : public AbstractLangStrategy
{

public:
    SuperColliderLangStrategy();


    // AbstractLangStrategy interface
public:
    QString get_sourceCode(const TreeItem *treeItem, const bool &richText) const;
    QString get_commentedSignature() const;
    QString get_Window(bool richText) const;
    bool recreatePixmap (const QString& textData, QPixmap &pixmap) const;
    void translateName(QString& nameOfElement);

    void paintElement(const TreeItem *t, QPainter *painter);
};


//---AutoIt language
class AutoItLangStrategy : public AbstractLangStrategy
{

public:
    AutoItLangStrategy();


    // AbstractLangStrategy interface
public:
    QString get_sourceCode(const TreeItem *treeItem, const bool &richText) const;
    QString get_commentedSignature() const;
    QString get_Window(bool richText) const;
    bool recreatePixmap (const QString& textData, QPixmap &pixmap) const;
    void translateName (QString &nameOfElement);

    void paintElement(const TreeItem *t, QPainter *painter);
};

#endif // LANGSTRATEGY_H
