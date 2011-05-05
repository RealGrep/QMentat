#-------------------------------------------------
#
# Project created by QtCreator 2010-12-04T00:22:27
#
#-------------------------------------------------

QT       += core gui webkit sql

TARGET = QMentat
TEMPLATE = app

QMAKE_CXXFLAGS_DEBUG=-std=c++0x

include(/home/michel/qtmmlwidget-2.4_1-opensource/src/qtmmlwidget.pri)

#LIBS += -lrandom

SOURCES += main.cpp\
        mainwindow.cpp \
    additionmodule.cpp \
    additionconfigframe.cpp \
    PracticeModule.cpp \
    practicemodule.cpp \
    multiplicationmodule.cpp \
    multiplicationconfigframe.cpp \
    subtractionmodule.cpp \
    subtractionconfigframe.cpp \
    divisionmodule.cpp \
    divisionconfigframe.cpp \
    questiondisplayform.cpp \
    powersmodule.cpp \
    powersconfigframe.cpp \
    rootsmodule.cpp \
    rootsconfigframe.cpp \
    questiondisplay.cpp \
    mathdisplayform.cpp \
    qulonglongvalidator.cpp

HEADERS  += mainwindow.h \
    additionmodule.h \
    additionconfigframe.h \
    practicemodule.h \
    multiplicationmodule.h \
    multiplicationconfigframe.h \
    subtractionmodule.h \
    subtractionconfigframe.h \
    divisionmodule.h \
    divisionconfigframe.h \
    questiondisplayform.h \
    powersmodule.h \
    powersconfigframe.h \
    rootsmodule.h \
    rootsconfigframe.h \
    questiondisplay.h \
    mathdisplayform.h \
    random.h \
    qulonglongvalidator.h

FORMS    += mainwindow.ui \
    additionconfigframe.ui \
    multiplicationconfigframe.ui \
    subtractionconfigframe.ui \
    divisionconfigframe.ui \
    questiondisplayform.ui \
    powersconfigframe.ui \
    rootsconfigframe.ui \
    mathdisplayform.ui

OTHER_FILES += \
    SubtractionIcon16x16.png \
    RootsIcon16x16.png \
    PowersIcon16x16.png \
    MultiplicationIcon16x16.png \
    DivisionIcon16x16.png \
    AdditionIcon16x16.png \
    applications_education.png

TRANSLATIONS += \
   qmentat_fr.ts \
   qmentat_es.ts
