#-------------------------------------------------
#
# Project created by QtCreator 2010-12-04T00:22:27
#
#-------------------------------------------------

QT       += core gui webkit sql

TARGET = QMentat
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

include(/home/michel/code/QMentat/qtmmlwidget-2.4_1-opensource/src/qtmmlwidget.pri)

#INCLUDEPATH
#LIBS += -lrandom
#DESTDIR
#DEFINES
#DEPENDPATH
#PKGCONFIG +=
#CONFIG += debug
#CONFIG(debug, debug|release) {
#    message("Debug mode")
#}

DESTDIR = ../QMentat-install

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
    exit.png \
    QMentat.png \
    Subtraction_32x32.png \
    Roots_32x32.png \
    Powers_32x32.png \
    Multiplication_32x32.png \
    Division_32x32.png \
    Addition_32x32.png \
    Doxyfile \
    qmentat_fr.ts \
    qmentat_es.ts \
    INSTALL

TRANSLATIONS += \
    qmentat_fr.ts \
    qmentat_es.ts

# INSTALLATION
target.path = $$DESTDIR

images.path = $$DESTDIR
images.files = *.png

translates.path = $$DESTDIR
translates.files = *.qm

INSTALLS = \
    target \
    images \
    translates
