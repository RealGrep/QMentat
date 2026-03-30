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

#define HELP_URL "qthelp://mike.dusseault.qmentat/doc/index.html"
#define QMENTAT_HELP_FILE "QMentat.qhc"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

#if defined(SHARE_DIR)
    QString docsFile = SHARE_DIR;
    docsFile.append("/");
    docsFile.append(QMENTAT_HELP_FILE);
#else
    QString docsFile = QStandardPaths::locate(
    QStandardPaths::AppDataLocation, QMENTAT_HELP_FILE);

    // Fallback to source/running directory
    if (docsFile.isEmpty())
    {
        //docsFile = QApplication::applicationDirPath() + "/documentation/QMentat.qhc";
        // Try installed location first
        docsFile = QStandardPaths::locate(QStandardPaths::AppDataLocation, "documentation/QMentat.qhc");

        // Fall back to development location
        if (docsFile.isEmpty())
        {
            docsFile = QString(QMENTAT_SOURCE_DIR) + "/documentation/QMentat.qhc";
        }
    }
#endif

    helpEngine = new QHelpEngine(docsFile, this);

    ui->helpBrowser->setHelpEngine(helpEngine);

    resize(600, 600);
    setModal(false);

    QHelpContentWidget *contentPanel = helpEngine->contentWidget();
    contentPanel->setItemsExpandable(true);
    contentPanel->expandAll();

    ui->splitter->insertWidget(0, (QWidget*)contentPanel);
    ui->splitter->setStretchFactor(0, 1);

    connect(helpEngine->contentWidget(),
            SIGNAL(linkActivated(const QUrl &)),
            ui->helpBrowser, SLOT(setSource(const QUrl &)));

    connect(helpEngine, &QHelpEngine::setupFinished, this, [this]() {
        ui->helpBrowser->setSource(QUrl(HELP_URL));
        helpEngine->contentWidget()->expandAll();
    });

    if (!helpEngine->setupData())
        qWarning() << "Help engine setup failed:" << helpEngine->error();
}

HelpDialog::~HelpDialog()
{
    delete ui;

    delete helpEngine;
    helpEngine = nullptr;
}
