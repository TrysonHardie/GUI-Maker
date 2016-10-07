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

#include "langstrategy.h"
#include "mainwindow.h"
#include "treeitem.h"
#include "guimakerscene.h"
#include "treeitem.h"

#include <QPainter>

const QString signa = "Generated with " GUI_MAKER_TITLE " " GUI_MAKER_SITE " " ;


AbstractLangStrategy::AbstractLangStrategy(QString langName, QString fileExtention):
    m_langName(langName),
    m_fileExtention(fileExtention)
{
    m_pMainWindow = MainWindow::getInstance();
    m_pGuiMakerScene = m_pMainWindow->guiMakerscene;

}

QString AbstractLangStrategy::get_pathToScFile(const QString &name) const
{
    return QString(":/data/HelpSources/%1/%2.schelp")
            .arg(m_langName).arg(name);
}

QString AbstractLangStrategy::get_CurrentLangName() const
{
    return m_langName;
}

QString AbstractLangStrategy::get_extention() const
{
    return m_fileExtention;
}





SuperColliderLangStrategy::SuperColliderLangStrategy(): AbstractLangStrategy("SuperCollider", ".scd")
{

}

QString SuperColliderLangStrategy::get_sourceCode(const TreeItem *treeItem, const bool &richText) const
{
//    because function is too complicated
    return treeItem->get_info_SuperCollider(richText);
}

QString SuperColliderLangStrategy::get_commentedSignature() const
{
    return QString("/*%1*/").arg(signa);
}

//Window("w", Rect(470, 0, 290, 230));
QString SuperColliderLangStrategy::get_Window(bool richText) const
{
    QString resultText, mainWindowVarName;
    mainWindowVarName = "w";

//    in the beg of the file must be list of all variables, if exist - "var w, somevar1, somevar2;"
    //    name of variable of mainWindow is mainWindowVarName == ('w')
    QString listOfVars = "\nvar " + mainWindowVarName;

    foreach (QGraphicsItem *item, m_pGuiMakerScene->items()) {
        if (TreeItem *element = qgraphicsitem_cast<TreeItem *>(item))
            if( !element->varName().isEmpty() )
            {
                listOfVars.append("," + element->varName());
            }
    }

    listOfVars.append(";\n");

    if( richText )
    {
        resultText = mainWindowVarName+" = <font color=\"blue\">Window</font>.new(\"gui\", Rect(100, 100, "+
                m_pMainWindow->subWindow_width_height()+
                ")).<font color=\"darkred\">front</font>;<br><br>";
    }
    else
    {
        resultText =  listOfVars + mainWindowVarName +" = Window.new(\"gui\", Rect(100, 100, "+
                m_pMainWindow->subWindow_width_height()+
                ")).front;\n\n";
    }

    return resultText;
}

// create blank pixmap, for drawing text on it
bool SuperColliderLangStrategy::recreatePixmap(const QString &textData, QPixmap &pixmap) const
{

    if (textData == "StaticText" || textData == "Button")
    {
        pixmap = QPixmap(128, 36);
        pixmap.fill(Qt::white);
    }
    else if (textData == "CompositeView")
    {
        pixmap = QPixmap(192, 128);
        pixmap.fill(Qt::gray);
    }
    else
        return false;

    return true;
}

void SuperColliderLangStrategy::translateName (QString &nameOfElement)
{
//    Are u sure about that?
    return;
}

void SuperColliderLangStrategy::paintElement(const TreeItem *t, QPainter *painter)
{
    const QString& m_typeName = t->m_typeName;
    const QList<TreeItem::Method>& methods = t->methods;
    const QRect& m_rect = t->m_rect;

    //    ToDo- complicated?
    if( m_typeName.contains( "Button" )){

        //    we actually get: ["yes",Color.grey, Color.white], ["no",Color.white, Color.grey]

        //        ---Find 'states' field and draw  a substring of first state

        const int j = 1; // index of 'states' field
        if(methods.at(j).value != "")
        {
            //                       get substring ... ["yes "] ... ["yes ",C olor
            int lastSymbolWeNeed = methods.at(j).value.indexOf("\"]");
            if(lastSymbolWeNeed == -1)
                lastSymbolWeNeed = methods.at(j).value.indexOf("\",C");

            QString stateStr = methods.at(j).value.left(lastSymbolWeNeed).remove(0,2);

            painter->drawText(QPointF(10, m_rect.height()/2), stateStr);

        }
        //                draw inner boundary of a Button
        QPen pen(QColor(50,90,255), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen );

        painter->setBrush(Qt::NoBrush);

        QRectF rectrect(2,2, m_rect.width()-2, m_rect.height()-2) ;
        painter->drawRect(rectrect);
    }
    else if (m_typeName == "StaticText")
    {
        //    ToDo- painter->setFont(QFont(methods.at(j).value));

        if(methods.at(0).value != "")
            painter->drawText(QPointF(10, 15), methods.at(0).value);
        else
            painter->drawText(QPointF(10, 15), "StaticText");

    }
    else //    add StaticText to CompositeView. methods.at(0).value - its a string
        if (m_typeName == "CompositeView" )
        {

            // fill background
            //        ---Find 'background' field
            for (int indexOfBackground = 0; indexOfBackground < methods.size(); ++indexOfBackground) {
                if( methods.at(indexOfBackground).property.contains("background", Qt::CaseInsensitive))
                {
                    if( methods.at(indexOfBackground).value != "" )
                    {
                        QString backgroundColor = methods.at(indexOfBackground).value;
//                        QColor bkColor = QColor(backgroundColor);
//                        bkColor.setNamedColor(backgroundColor);
                        painter->fillRect(m_rect, QColor(backgroundColor));

                    }
                    break;

                }
            }

            // draw text
            if(methods.at(0).value != "")
                painter->drawText(QPointF(10, 15), methods.at(0).value);

        }

}



//---AutoIt language

AutoItLangStrategy::AutoItLangStrategy(): AbstractLangStrategy("AutoIt", ".au3")
{

}

QString AutoItLangStrategy::get_sourceCode(const TreeItem *treeItem, const bool &richText) const
{
    return treeItem->get_info_AutoIt(richText);

}

QString AutoItLangStrategy::get_commentedSignature() const
{
    return QString("; %1").arg(signa);

}

// Local $hGUI = GUICreate("gui", 300, 200)
QString AutoItLangStrategy::get_Window(bool richText) const
{

    QString resultText, br, mainWindowVarName, constants;

    mainWindowVarName = "hGUI";

    //            including some libraries
    constants = "\n#include <EditConstants.au3>\n#include <GUIConstantsEx.au3>\n#include <StaticConstants.au3>\n#include <WindowsConstants.au3> "
            ;

    if( richText )
    {
        resultText ="Local $"+ mainWindowVarName +" = <font color=\"blue\">GUICreate</font>(\"gui\", "+
                m_pMainWindow->subWindow_width_height()+
                ")<br><br>";
        br = "<br>";
    }
    else
    {
        resultText =constants + "\nLocal $"+ mainWindowVarName +" = GUICreate(\"gui\", "+
                m_pMainWindow->subWindow_width_height()+
                ")\n";
        br= "\n";
    }

    //        GUISetState(@SW_SHOW, $hGUI)
    resultText.append(QString("GUISetState(@SW_SHOW, $%2)%1"
).arg(br).arg(mainWindowVarName));

    return resultText;
}

// create blank pixmap, for drawing text on it
bool AutoItLangStrategy::recreatePixmap(const QString &textData, QPixmap &pixmap) const
{

    if (textData == "GUICtrlCreateLabel" || textData == "GUICtrlCreateButton")
    {
//        duplicate of code here
        pixmap = QPixmap(128, 36);
        pixmap.fill(Qt::white);
    }
//    else if (textData == "CompositeView")
//    {
//        pixmap = QPixmap(192, 128);
//        pixmap.fill(Qt::gray);
//    }
    else
        return false;

    return true;
}

void AutoItLangStrategy::translateName(QString& nameOfElement)
{
    if(nameOfElement.isEmpty())
        return;

    //    convert fileName
    if (nameOfElement == "Button")
        nameOfElement = "GUICtrlCreateButton";
    else if (nameOfElement == "ListView")
        nameOfElement = "GUICtrlCreateList";
    else if (nameOfElement == "StaticText")
        nameOfElement = "GUICtrlCreateLabel";

    else if (nameOfElement == "CheckBox")
        nameOfElement = "GUICtrlCreateCheckBox";
    else if (nameOfElement == "Knob")
        return;
//        nameOfElement = "";
    else if (nameOfElement == "NumberBox")
        return;
//        nameOfElement = "";

    else if (nameOfElement == "PopUpMenu")
        return;
//        nameOfElement = "GUICtrlCreateCombo";
    else if (nameOfElement == "Slider")
        nameOfElement = "GUICtrlCreateSlider";
    else if (nameOfElement == "TextField")
        nameOfElement = "GUICtrlCreateEdit";
    else
        return;
//    else if (nameOfElement == "CompositeView")
//        nameOfElement = "";

}

void AutoItLangStrategy::paintElement(const TreeItem *t, QPainter *painter)
{
    const QString& m_typeName = t->m_typeName;
    const QList<TreeItem::Method>& methods = t->methods;
    const QRect& m_rect = t->m_rect;

    if( methods.at(0).property == "text"
            && !methods.at(0).value.isEmpty())
    {
        painter->drawText(QPointF(10, m_rect.height()/2), methods.at(0).value);
    }
    if( m_typeName.contains( "Button" )){
        //                draw inner boundary of a Button
        QPen pen(QColor(50,90,255), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen );

        painter->setBrush(Qt::NoBrush);

        QRectF rectrect(2,2, m_rect.width()-2, m_rect.height()-2) ;
        painter->drawRect(rectrect);
    }

}
