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

//hard
QString SuperColliderLangStrategy::get_sourceCode(const TreeItem *treeItem, const bool &richText) const
{
    const QString& m_className = treeItem->m_className;
    const QList<TreeItem::Method>& methods = treeItem->methods;
    const QRect& m_rect = treeItem->m_rect;
    const QString& m_varName = treeItem->m_varName;

    QString classTag, methodTag,methodTag2, br, varname;
    //        Qt's text widgets are able to display rich text: HTML 4 markup
    if (richText)
    {
        classTag = "<font color=\"blue\">" + m_className + "</font>";
        methodTag = "<font color=\"darkred\">";
        methodTag2 = "</font>";
        br = "<br>";
    }
    else
    {
        classTag = m_className;
        //        methodTag = "";
        //        methodTag2 = "";
        br = "\n";
    }

    //name of variable -- "mvar = "
    if (m_varName != "")
    {
        QString txt_before_CompositeView("");
        //        add  "// --- varname container View"  before CompositeView
        if (m_className == "CompositeView")
            txt_before_CompositeView = methodTag + "// ---" + m_varName+
                    " container View" + methodTag2 + br;

        varname = txt_before_CompositeView + m_varName+" = ";

    }

    //    Constructor - m = PopUpMenu(w,Rect(10,10,180,20))
    QString text(varname + classTag + "(" + treeItem->parentItemvarName() +
                 ", Rect(" + treeItem->rect_of_element() + "))"+br);

    //    add StaticText to CompositeView
    if (m_className == "CompositeView")
    {
        //        ---Find 'background' field
        //        int indexOfBackground = methods.indexOf("background");//        need an implementation of operator==()
        for (int indexOfBackground = 0; indexOfBackground < methods.size(); ++indexOfBackground) {
            if( methods.at(indexOfBackground).property.contains("background", Qt::CaseInsensitive))
            {
                if( methods.at(indexOfBackground).value != "" )
                {
                    QString colorTag = methods.at(indexOfBackground).value;
                    if (richText)
                        colorTag = "<font color="+colorTag+">" + colorTag + methodTag2;
                    text.append("." + methodTag +methods.at(indexOfBackground).property+ methodTag2 +
                                "_(Color.fromHexString(\"" + colorTag +  "\"))"+br);

                }
                break;

            }
        }



        //        ---Find 'background' field
        //ToDO- complicated!
        //        ---get font size -
        //        if string is empty - dont add StaticText
        if (!methods.at(0).value.isEmpty())
        {
            //        1. Find 'Font' field in the methods. Now we get font = "12|Arial"

            //        int methodIndexposition = 4;//        is it 4?

            for (int methodIndexposition = 0; methodIndexposition < methods.size(); ++methodIndexposition) {
                if( methods.at(methodIndexposition).property.contains("Font", Qt::CaseInsensitive))
                {
                    QString fontSize("12");
                    //        2. get (0) section of the string, separated by the ("|")
                    if (methods.at(methodIndexposition).value != "")
                    {
                        fontSize = methods.at(methodIndexposition).value.section('|', 0, 0);
                    }

                    text.append(";StaticText(" + m_varName +
                                ", Rect(10,10, " +QString::number(m_rect.width()) + "," + fontSize + "))"+br);
                    break;

                }
            }


        }
    }

    //methods
    for (int i = 0; i < methods.size(); ++i) {
        if( methods.at(i).value != "" )
        {

            if(methods.at(i).typeOf_argument.contains("Array")){
                //    we get: Sine, Saw, Noise, Pulse
                QString m_txt = "[";
                QStringList items =  methods.at(i).value.split(",");
                int rowcount = items.size();

                //                Array of Colors
                if( methods.at(i).typeOf_argument.contains("Colors") )
                {
                    //                    m_txt;

                    for (int row = 0; row < rowcount; ++row) {
                        QString colorTag = items.at(row);
                        if (richText)
                            colorTag = "<font color="+colorTag+">" + colorTag + methodTag2;
                        m_txt.append(br+"Color.fromHexString(\"" + colorTag +  "\"),");
                    }
                }
                else if( m_className == "Button" ){
                    //    we actually get: ["yes", Color.grey, Color.white], ["no", Color.white, Color.grey]
                    m_txt.append( methods.at(i).value + QString(","));

                    //                    bad - [["yes"\n, Color.grey\n, Color.white]\n, ["no"\n, Color.white\n, Color.grey]\n,
                    //                    for (int row = 0; row < rowcount; ++row) {
                    //                        m_txt.append(items.at(row) + br + QString(","));
                    //                        }
                }
                else
                {
                    //                    m_txt = "[";

                    for (int row = 0; row < rowcount; ++row) {
                        m_txt.append("\"" + items.at(row) + QString("\","));
                    }
                }

                //    remove last ","
                m_txt.replace(m_txt.size()-1, 1, "]");

                //    we have: ["Sine","Saw","Noise","Pulse"]
                text.append("." + methodTag +methods.at(i).property+ methodTag2 +
                            "_(" + m_txt +  ")"+br);
            }
            else if(methods.at(i).typeOf_argument.contains("Float")
                    || methods.at(i).typeOf_argument.contains("Integer")
                    || methods.at(i).typeOf_argument.contains("Number"))
            {
                //                .font_(Font("Courier", 13));
                text.append("." + methodTag +methods.at(i).property+ methodTag2 +
                            "_(" + methods.at(i).value +  ")"+br);
            }
            else if( methods.at(i).typeOf_argument.contains("Font") )
            {
                //                we get font = "12|Arial"
                QStringList font = methods.at(i).value.split("|");
                //                .font_(Font("Courier", 13));
                text.append("." + methodTag +methods.at(i).property+ methodTag2 +
                            "_(Font(\"" + font.at(1) +"\"," + font.at(0) +  "))"+br);
            }
            else if( methods.at(i).typeOf_argument.contains("Color") )
            {
                QString colorTag = methods.at(i).value;
                if (richText)
                    colorTag = "<font color="+colorTag+">" + colorTag + methodTag2;
                text.append("." + methodTag +methods.at(i).property+ methodTag2 +
                            "_(Color.fromHexString(\"" + colorTag +  "\"))"+br);
            }
            else if( methods.at(i).typeOf_argument.contains("Boolean") )
            {
                text.append("." + methodTag +methods.at(i).property+ methodTag2 +
                            "_(" + methods.at(i).value +  ")"+br);
            }

            else
            {
                //                .string_("Courier");
                text.append("." + methodTag +methods.at(i).property+ methodTag2 +
                            "_(\"" + methods.at(i).value +  "\")"+br);
            }
        }
    }

    text.append(";"+br);

    return text;

}

QString SuperColliderLangStrategy::get_commentedSignature() const
{
    return QString("/*%1*/").arg(signa);
}

//Window("w", Rect(470, 0, 290, 230));
QString SuperColliderLangStrategy::get_Window(const bool richText) const
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
    const QString& m_className = treeItem->m_className;
    const QList<TreeItem::Method>& methods = treeItem->methods;
//    const QRect& m_rect = treeItem->m_rect;
    const QString& m_varName = treeItem->m_varName;


    //    Rich text?
    QString classTag, methodTag,methodTag2, br, varname;
    if (richText)
    {
        classTag = "<font color=\"blue\">" + m_className + "</font>";
        methodTag = "<font color=\"darkred\">";
        methodTag2 = "</font>";
        br = "<br>";
    }
    else
    {
        classTag = m_className;
        br = "\n";
    }

    //name of variable
    if (m_varName != "")
        varname = "Local $" + m_varName+" = ";

//    Constructor
//    Local $idClose = GUICtrlCreateButton("Close",  210, 170, 85, 25)
    QString text = (varname + classTag + "(\"" + methodTag + methods.at(0).value + methodTag2 + "\", " +
                    treeItem->rect_of_element() + ")"+br);

//    GUICtrlSetFont(-1, 9, $FW_NORMAL, $GUI_FONTUNDER, $sFont)
//    QString text = (varname + classTag + "(" +
//                    QString::number(m_rect.x()) + "," + QString::number(m_rect.y()) +"," +
//                    QString::number(m_rect.width()) + "," + QString::number(m_rect.height()) +
//                    ")"+br);

    //methods
    for (int i = 0; i < methods.size(); ++i) {
        if( methods.at(i).value != "" )
        {
            if( methods.at(i).typeOf_argument.contains("Array") )
            {
                //                    	GUICtrlSetData(-1, "datas|data1|data2|")
                QString items = methods.at(i).value;
                        items.replace(",", "|");

                text.append(methods.at(i).property + "(-1, \"" +
                        items +
                        "\")"+br);
            }
            else if( methods.at(i).typeOf_argument.contains("Color") )
            {
//                GUICtrlSetBkColor($idLabel, 0x0F0F0F)

                QString colorTag = methods.at(i).value;
//                    colorTag = "<font color="+colorTag+">" + colorTag + methodTag2;

                colorTag.remove(0,1);
                if (richText)
                    colorTag = "<font color=#"+colorTag+">" + colorTag + methodTag2;
//                    else
//                        colorTag = colorTag.prepend("0x");
                text.append(methods.at(i).property + "(-1, 0x" + colorTag +")"+br);
            }
            else if( methods.at(i).typeOf_argument.contains("Font") )
            {
//                font = "12|Arial"
                QStringList font = methods.at(i).value.split("|");
//                GUICtrlSetFont(-1, 9, $FW_NORMAL, $GUI_FONTUNDER, $sFont)
                text.append(methods.at(i).property + "(-1, "+font[0] +
                        ", $FW_NORMAL, $GUI_FONTUNDER,\"" +
                        font[1] +
                        "\")"+br);
            }
            //                else if( methods.at(i).typeOf_argument.contains("String") )
            //                {
            //                }
        }
//            if( methods.at(i).typeOf_argument.contains("Fuction") )
//            {
//            }
//            if( methods.at(i).typeOf_argument.contains("Integer") )
//            {
//            }
    }

    text.append(br);

    return text;


}

QString AutoItLangStrategy::get_commentedSignature() const
{
    return QString("; %1").arg(signa);

}

// Local $hGUI = GUICreate("gui", 300, 200)
QString AutoItLangStrategy::get_Window(const bool richText) const
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
        resultText =constants + "\n\nLocal $"+ mainWindowVarName +" = GUICreate(\"gui\", "+
                m_pMainWindow->subWindow_width_height()+
                ")\n";
        br= "\n";
    }

    //        GUISetState(@SW_SHOW, $hGUI)
    resultText.append(QString("GUISetState(@SW_SHOW, $%2)%1%1"
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

QString AutoItLangStrategy::get_endCode(const bool richText) const
{
    QString resultText, br;

    if( richText )
    {
        br = "<br>";
    }
    else
    {
        br= "\n";
    }
    //; ---Display the GUI.
    //    ; Loop until the user exits.
    //    While 1
    //        Switch GUIGetMsg()
    //            Case $GUI_EVENT_CLOSE
    //                ExitLoop
    //        EndSwitch
    //    WEnd
    resultText.append(QString("While 1%1Switch GUIGetMsg()%1Case $GUI_EVENT_CLOSE %1ExitLoop%1EndSwitch%1WEnd%1%1"
).arg(br));

    return resultText;

}
