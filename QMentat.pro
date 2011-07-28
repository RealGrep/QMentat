#-------------------------------------------------
#
# Project created by QtCreator 2010-12-04T00:22:27
#
#-------------------------------------------------

QT       += core gui

TARGET = QMentat
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x
LIBS += -lgmpxx -lgmp
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
    qulonglongvalidator.cpp \
    pieview.cpp \
    statisticsdialog.cpp \
    bigfixedpoint.cpp \
    qbigfixedvalidator.cpp \
    licensedialog.cpp \
    preferencesdialog.cpp \
    preferences.cpp

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
    qulonglongvalidator.h \
    FixedPoint.h \
    pieview.h \
    statisticsdialog.h \
    bigfixedpoint.h \
    qbigfixedvalidator.h \
    licensedialog.h \
    preferencesdialog.h \
    preferences.h \
    preferenceslistener.h

FORMS    += mainwindow.ui \
    additionconfigframe.ui \
    multiplicationconfigframe.ui \
    subtractionconfigframe.ui \
    divisionconfigframe.ui \
    questiondisplayform.ui \
    powersconfigframe.ui \
    rootsconfigframe.ui \
    mathdisplayform.ui \
    statisticsdialog.ui \
    licensedialog.ui \
    preferencesdialog.ui

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
    INSTALL \
    documentation/QMentat.qhp \
    documentation/QMentat.qhcp \
    documentation/index.html \
    documentation/about.txt

TRANSLATIONS += \
    qmentat_fr.ts \
    qmentat_es.ts

# INSTALLATION
target.path = $$DESTDIR

script.path = $$DESTDIR
script.files = QMentat.sh

appicon.path = $$DESTDIR
appicon.files = QMentat.png

images.path = $$DESTDIR
images.files = *.png

translates.path = $$DESTDIR
translates.files = *.qm

docs.path = $$DESTDIR
docs.files = documentation/QMentat.qhc LICENSE

INSTALLS = \
    target \
    script \
    appicon \
 #   images \
 #   translates \
    docs

RESOURCES += \
    qmentat.qrc
