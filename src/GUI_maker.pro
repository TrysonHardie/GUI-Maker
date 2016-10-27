#-------------------------------------------------
#
# Project created by QtCreator 2016-07-19T16:02:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI_maker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    piecesmodel.cpp \
    treemodel.cpp \
    treeitem.cpp \
    tablemodel.cpp \
    spinboxdelegate.cpp \
    arrayeditorbutton.cpp \
    arrayeditor.cpp \
    fonteditorbutton.cpp \
#    coloreditorbutton.cpp \
    abstracteditorbutton.cpp \
    settingsdialog.cpp \
    arrayofcolorseditorbutton.cpp \
    guimakerscene.cpp \
    stateseditor.cpp \
    langstrategy.cpp \
    undoRedoCommands.cpp \
    windowparametersdialog.cpp

HEADERS  += mainwindow.h \
    piecesmodel.h \
    treemodel.h \
    treeitem.h \
    tablemodel.h \
    spinboxdelegate.h \
    arrayeditorbutton.h \
    arrayeditor.h \
    fonteditorbutton.h \
#    coloreditorbutton.h \
    abstracteditorbutton.h \
    settingsdialog.h \
    arrayofcolorseditorbutton.h \
    guimakerscene.h \
    stateseditor.h \
    langstrategy.h \
    undoRedoCommands.h \
    windowparametersdialog.h


FORMS    += mainwindow.ui

RESOURCES += \
    guielements.qrc
