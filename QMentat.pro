#-------------------------------------------------
#
# Project created by QtCreator 2010-12-04T00:22:27
#
#-------------------------------------------------

QT       += core gui

TARGET = bin/QMentat
TEMPLATE = app

CONFIG += help

#QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -std=c++0x
LIBS += -lgmpxx -lgmp

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

# WARNING: Never leave any DESTDIR variable uncommented or you will break some of the
#          automated package builds (i.e. Ubuntu, Arch, etc)
#DESTDIR = /home/michel/code/QMentat-install
#DESTDIR = /usr
#message($$DESTDIR)

PREFIX = /usr

#QMAKE_CXXFLAGS += -DSHARE_DIR=\'"$$DESTDIR/share/QMentat"\'
#DEFINES += SHARE_DIR=\\\"$$DESTDIR/share/QMentat\\\"

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
#    mathdisplayform.cpp \
    qulonglongvalidator.cpp \
    pieview.cpp \
    statisticsdialog.cpp \
    bigfixedpoint.cpp \
    qbigfixedvalidator.cpp \
#    licensedialog.cpp \
    preferencesdialog.cpp \
    preferences.cpp \
    helpbrowser.cpp \
    helpdialog.cpp

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
#    mathdisplayform.h \
    random.h \
    qulonglongvalidator.h \
    FixedPoint.h \
    pieview.h \
    statisticsdialog.h \
    bigfixedpoint.h \
    qbigfixedvalidator.h \
#    licensedialog.h \
    preferencesdialog.h \
    preferences.h \
    preferenceslistener.h \
    helpbrowser.h \
    helpdialog.h

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
#    licensedialog.ui \
    preferencesdialog.ui \
    helpdialog.ui

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
    documentation/about.txt \
    README

TRANSLATIONS += \
    qmentat_fr.ts \
    qmentat_es.ts

# INSTALLATION
target.path = $$PREFIX/bin

script.path = $$PREFIX/bin
script.files = QMentat.sh

appicon.path = $$PREFIX/share/icons
appicon.files = QMentat.png

desktop.path = $$PREFIX/share/applications
desktop.files = qmentat.desktop

images.path = $$PREFIX/share/QMentat
images.files = *.png

translates.path = $$PREFIX/share/Qmentat
translates.files = *.qm

docs.path = $$PREFIX/share/QMentat
docs.files = documentation/QMentat.qhc documentation/QMentat.qch LICENSE

INSTALLS = \
    target \
#    script \
    appicon \
    desktop \
#   images \
#   translates \
    docs

RESOURCES += \
    qmentat.qrc

!isEmpty(TRANSLATIONS) {
  isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
  }

  isEmpty(TS_DIR):TS_DIR = Translations

  TSQM.name = lrelease ${QMAKE_FILE_IN}
  TSQM.input = TRANSLATIONS
  TSQM.output = $$TS_DIR/${QMAKE_FILE_BASE}.qm
  TSQM.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN}
  TSQM.CONFIG = no_link
  QMAKE_EXTRA_COMPILERS += TSQM
  PRE_TARGETDEPS += compiler_TSQM_make_all
} else:message(No translation files in project)
