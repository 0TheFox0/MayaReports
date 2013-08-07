#-------------------------------------------------
#
# Project created by QtCreator 2013-07-21T18:08:34
#
#-------------------------------------------------

QT       += core gui printsupport xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    customlabel.cpp \
    container.cpp \
    paper.cpp \
    section.cpp \
    detailsection.cpp \
    roundedrect.cpp \
    reportline.cpp \
    reportview.cpp \
    codebar.cpp \
    reportimage.cpp \
    editcodebardlg.cpp \
    pageheadersection.cpp \
    sectioneditordlg.cpp \
    editdetailsecdlg.cpp \
    edittextdlg.cpp \
    editlinedlg.cpp \
    editrectdlg.cpp \
    editpaperdlg.cpp \
    editdinamycitemdlg.cpp \
    reportfield.cpp \
    editfielddlg.cpp \
    editimagedlg.cpp \
    reportrenderer.cpp \
    relationalfield.cpp \
    editrelationalfield.cpp

HEADERS  += mainwindow.h \
    customlabel.h \
    container.h \
    paper.h \
    section.h \
    detailsection.h \
    roundedrect.h \
    reportline.h \
    reportview.h \
    codebar.h \
    reportimage.h \
    editcodebardlg.h \
    pageheadersection.h \
    sectioneditordlg.h \
    editdetailsecdlg.h \
    edittextdlg.h \
    editlinedlg.h \
    editrectdlg.h \
    editpaperdlg.h \
    editdinamycitemdlg.h \
    reportfield.h \
    editfielddlg.h \
    editimagedlg.h \
    reportrenderer.h \
    relationalfield.h \
    editrelationalfield.h

FORMS    += mainwindow.ui \
    editcodebardlg.ui \
    sectioneditordlg.ui \
    editdetailsecdlg.ui \
    edittextdlg.ui \
    editlinedlg.ui \
    editrectdlg.ui \
    editpaperdlg.ui \
    editfielddlg.ui \
    editimagedlg.ui \
    editrelationalfield.ui

RESOURCES += \
    resource.qrc
