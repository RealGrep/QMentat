#include "helpdialog.h"
#include "ui_helpdialog.h"

#include <QtGui>
#include <QtHelp>

//#define XSTR(x) #x
//#define STR(x) XSTR(x)

#if !defined(SHARE_DIR)
#   define SHARE_DIR "/home/michel/code/QMentat-install/"
#endif

#define HELP_URL "qthelp://mike.dusseault.qmentat/doc/index.html"
#define QMENTAT_HELP_FILE "QMentat.qhc"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    qDebug() << SHARE_DIR;

    QString docsFile = SHARE_DIR;
    docsFile.append("/");
    docsFile.append(QMENTAT_HELP_FILE);

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
