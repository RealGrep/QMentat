/* Copyright (c) 2013 Mike Dusseault
 *
 * This file is part of QMentat.
 *
 * QMentat is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QMentat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QMentat.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "helpdialog.h"
#include "ui_helpdialog.h"

#include <QtGui>
#include <QtHelp>

//#define XSTR(x) #x
//#define STR(x) XSTR(x)

//#if !defined(SHARE_DIR)
//#   define SHARE_DIR "/home/michel/code/QMentat-install/"
//#endif

#define HELP_URL "qthelp://mike.dusseault.qmentat/doc/index.html"
#define QMENTAT_HELP_FILE "QMentat.qhc"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    //qDebug() << SHARE_DIR;

#if defined(SHARE_DIR)
    QString docsFile = SHARE_DIR;
    docsFile.append("/");
    docsFile.append(QMENTAT_HELP_FILE);
#else
    QString docsFile = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();
    docsFile.append("/../share/QMentat/");
    docsFile.append(QMENTAT_HELP_FILE);
#endif
    //qDebug() << docsFile;

    helpEngine = new QHelpEngine(docsFile, this);
    helpEngine->setupData();

    ui->helpBrowser->setHelpEngine(helpEngine);

    resize(600, 600);
    setModal(false);

    QHelpContentWidget *contentPanel = helpEngine->contentWidget();
    contentPanel->setItemsExpandable(true);
    contentPanel->expandAll();

    ui->splitter->insertWidget(0, (QWidget*)contentPanel);
    ui->splitter->setStretchFactor(0, 1);
    ui->helpBrowser->setSource(QUrl(HELP_URL));

    connect(helpEngine->contentWidget(),
            SIGNAL(linkActivated(const QUrl &)),
            ui->helpBrowser, SLOT(setSource(const QUrl &)));
}

HelpDialog::~HelpDialog()
{
    delete ui;

    delete helpEngine;
    helpEngine = 0;
}
