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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <cassert>
#include <QtGui>
#include <QChar>
#include "practicemodule.h"
#include "additionmodule.h"
#include "subtractionmodule.h"
#include "multiplicationmodule.h"
#include "divisionmodule.h"
#include "powersmodule.h"
#include "rootsmodule.h"
#include "questiondisplayform.h"
#include "statisticsdialog.h"
#include "licensedialog.h"
#include "preferencesdialog.h"
#include "preferences.h"
#include "random.h"
//#include <QtSql/QtSql>
#include <QDebug>

#include "helpdialog.h"

//#define DOCS_PATH "/home/michel/code/QMentat/documentation/QMentat.qhc"

// For seeding qrand
#if defined(Q_OS_LINUX)
#   include <fstream>   // Linux only - for reading urandom
#else
#   include <QDateTime>
#endif

/*! \class MainWindow
 * \brief The MainWindow of the entire application.
 * \param parent The parent QWidget.
 *
 * \author Mike Dusseault <mike.dusseault@gmail.com>
 */
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("michel.dusseault");
    //QCoreApplication::setOrganizationName("Synergem");
    QCoreApplication::setApplicationName("QMentat");

    ui->setupUi(this);

    // Initialize sane values
    answerGiven = 0;
    totalQuestions = 0;
    totalCorrect = 0;
    totalWrong = 0;
    currentTab = 0;
#ifdef USE_ASSISTANT
    assistant = 0;
#endif

    // Initialize qrand seed
    qsrand(getSeed() % 1000000);

    // Load up the default module
    module = 0;
    moduleChange(new AdditionModule(this));

    ui->settingsTab->layout()->addWidget(module->getConfigFrame());

    // Restore saved settings
    readSettings();

    // Make sure the user accepts the license agreement on first run
#ifdef SHOW_LICENSE
    QSettings settings;
    bool licenseAccepted = settings.value("licenseAccepted", false).toBool();
    if (!licenseAccepted)
    {
        LicenseDialog license(this);
        int res = license.exec();

        licenseAccepted = (res == 1) ? true : false;
        settings.setValue("licenseAccepted", licenseAccepted);

        // Still haven't accepted? Bail.
        if (!licenseAccepted)
        {
            exit(0);
        }
    }
#endif

    // Kick off first question
    newQuestion();
    Preferences::getInstance().addListener(this);
    ui->lineEdit->setFont(Preferences::getInstance().getAnswerFont());
    ui->lineEdit->setFocus();
}

MainWindow::~MainWindow()
{
    writeSettings();

    Preferences::getInstance().removeListener(this);

    delete module;
    module = 0;

#ifdef USE_ASSISTANT
    if (assistant && assistant->state() == QProcess::Running) {
            assistant->terminate();
            assistant->waitForFinished(3000);
        }
        delete assistant;
#endif

    delete ui;
}

/*! Gets a seed from an appropriate entropy source.
 */
quint64 MainWindow::getSeed()
{
    quint64 seed;
#if defined(Q_OS_LINUX)
    std::ifstream urandom;
    urandom.open("/dev/urandom");
    urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));
    urandom.close();
#else // also available: Q_OS_WIN32 and Q_OS_MAC
    seed = QDateTime::currentMSecsSinceEpoch();
#endif
    return seed;
}

/*
void MainWindow::testSQL()
{
      QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
      db.setHostName("localhost");
      db.setDatabaseName("qmentat_data.sqlite");
      db.setUserName("default");
      //db.setPassword("1uTbSbAs");
      bool ok = db.open();
      if (ok) {
         std::cerr << "Database opened" << std::endl;
      }

      QSqlQuery query;
      bool ret = false;
      if (ok)
      {

         ret = query.exec("create table settings "
                          "(id integer primary key, "
                          "property varchar(20), "
                          "text varchar(30), "
                          "number integer)");
      }
      if (ret == false) {
         std::cerr << "Table probably already there." << std::endl;
      }

      query.exec("SELECT number FROM settings WHERE property = 'addition_firstmax'");
      if (query.next()) {
         int number = query.value(0).toInt();
         std::cerr << "Number: " << number << std::endl;
      } else {
         ret = false;
         if (ok)
         {
            QSqlDatabase::database().transaction();
            ret = query.exec("INSERT INTO settings (id, property, number) "
                             "VALUES (NULL, 'addition_firstmax', 42)");
         }
         if (ret == false) {
            std::cerr << "Urk, couldn't insert into table!" << std::endl;
         } else {
            QSqlDatabase::database().commit();
         }
      }

      //QSqlDatabase::database().transaction();
      //query.finish();
      //query.clear();
      QSqlQuery query2;

      if (query2.exec("SELECT property, number FROM settings") == false) {
         std::cerr << "Can't query DB for listing of rows!" << std::endl;
         QSqlError error = query2.lastError();
         std::cerr << "ERROR: " << error.text().toStdString() << std::endl;
      }
      //if (query.first()) {
      //   int val = query.value(1).toInt();
         //QString text = query.value(2).toString();
      //   std::cerr << "FIRST VAL: " << val << std::endl;
      //}
      while (query2.next()) {
         //QString property = query.value(0).toString();
         int val = query2.value(1).toInt();
         //QString text = query.value(2).toString();
         std::cerr << "VAL: " << val << std::endl;
               //<< "]" << std::endl;
      }

      //QSqlQuery query;
      //query.exec("CREATE TABLE ");
      //bool result = query.exec("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
      //std::cerr << "Result: " << result << std::endl;
}
*/

/*! Reads the saved Qt settings and restores to saved state.
 */
void MainWindow::readSettings() {
    QSettings settings;
    qDebug() << "Settings file: " << settings.fileName() << endl;
    QPoint pos = settings.value("pos", this->pos()).toPoint();
    QSize size = settings.value("size", this->sizeHint()).toSize();
    QByteArray state = settings.value("state", QByteArray()).toByteArray();
    restoreState(state);
    resize(size);
    move(pos);

    Preferences::getInstance().restore();
}

/*! Writes current Qt settings and saves them.
 */
void MainWindow::writeSettings() {
    // Save postion/size of main window
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("state", saveState());

    Preferences::getInstance().save();
}


/*! Gets a new question and displays it.
 */
void MainWindow::newQuestion()
{
    assert(module != 0);
    QString q = module->question();

    module->getDisplayFrame()->setText(q);
    ui->lineEdit->clear();
}

bool MainWindow::answerValid(const QString& answerGiven)
{

    bool first_char = true;
    bool saw_decimal = false;
    for (int i = 0; i < answerGiven.size(); i++)
    {
        QChar c = answerGiven[i];
        // If we see a + or - and it's not the first character we see, not valid.
        if (!first_char && (c == QChar('-') || c == QChar('+')))
        {
            return false;
        // Ensure only one decimal point
        } else if (c == QLocale::system().decimalPoint()) {
            if (saw_decimal) {
                return false;
            }
            saw_decimal = true;
        // Otherwise, if it's not a digit or a group separator, not valid.
        } else if (!c.isDigit() &&
                   c != QLocale::system().groupSeparator())
        {
            return false;
        }
        first_char = false;
    }
    return true;
}

/*! Return pressed, so verify the answer given and display result.
 */
void MainWindow::on_lineEdit_returnPressed()
{
    assert(module != 0);

    // Answer as integer
    QString answerGiven = ui->lineEdit->text().trimmed();
    if (answerGiven.size() == 0)
    {
        answerGiven = "0";
    }

    // Handle correct or not
    if (answerValid(answerGiven) && module->isCorrect(answerGiven))
    {
        ui->textEdit->setText(tr("Correct!"));
        this->totalCorrect++;
    } else {
        ui->textEdit->setText(tr("Wrong! %1").arg(module->getAnswerString()));
        this->totalWrong++;
    }
    this->totalQuestions++;
    statusBar()->showMessage(tr( "Total: %1  Correct/Wrong: %2/%3  %4%5 success rate" )
                             .arg(totalQuestions )
                             .arg(totalCorrect)
                             .arg(totalWrong)
                             .arg(((double)totalCorrect / (double)totalQuestions) * 100.0F, 0, 'f', 2)
                             .arg(QLocale::system().percent()));

    newQuestion();
    ui->lineEdit->setFocus();
}

/*! Method to change the currently active module to another module (addition,
 *    division, etc).
 * \param module The PracticeModule module which will be active, replacing
 *    the old one.
 */
void MainWindow::moduleChange(PracticeModule *mod) {
    assert(mod != 0);

    // Load the config and display frames
    if (module != 0) {
        ui->settingsTab->layout()->removeWidget(mod->getConfigFrame());
        module->getConfigFrame()->close();

        ui->displayPane->layout()->removeWidget(mod->getDisplayFrame());
        module->getDisplayFrame()->close();

        delete module;
        module = 0;
    }

    // Grab new module and get new config and display frames
    module = mod;
    ui->settingsTab->layout()->addWidget(module->getConfigFrame());
    ui->displayPane->layout()->addWidget(module->getDisplayFrame());

    assert(module != 0);

    newQuestion();
}

/*! Swap to addition practice.
 */
void MainWindow::on_actionAddition_triggered()
{
    moduleChange(new AdditionModule(this));
}

/*! Swap to multiplication practice.
 */
void MainWindow::on_actionMultiplication_triggered()
{
    moduleChange(new MultiplicationModule(this));
}

/*! Swap to subtraction practice.
 */
void MainWindow::on_actionSubtraction_triggered()
{
    moduleChange(new SubtractionModule(this));
}

/*! Swap to division practice.
 */
void MainWindow::on_actionDivision_triggered()
{
    moduleChange(new DivisionModule(this));
}

/*! Swap to powers practice.
 */
void MainWindow::on_actionPowers_triggered()
{
    moduleChange(new PowersModule(this));
}

/*! Swap to roots practice.
 */
void MainWindow::on_actionRoots_triggered()
{
    moduleChange(new RootsModule(this));
}

/*! The About box.
 * \todo Make a fancier about box
 */
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About QMentat"), tr("QMentat\n\nWritten by Mike Dusseault\nCopyright 2013 Mike Dusseault.\n\nQMentat is released under the GPL version 3."));
}

void MainWindow::on_actionStatistics_triggered()
{
    StatisticsDialog stats;
    stats.set(totalCorrect, totalWrong);
    stats.exec();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    // Changed to main tab (from Settings tab)
    if ((currentTab == 1) && (index == 0))
    {
        bool ok = module->applyConfig();
        if (!ok)
        {
            currentTab = 1;
            ui->tabWidget->setCurrentIndex(currentTab);
        } else {
            currentTab = 0;
        }
    } else {
        currentTab = index;
    }
}

void MainWindow::on_actionContents_triggered()
{
    HelpDialog *helpDialog = new HelpDialog(this);
    helpDialog->show();

#ifdef USE_ASSISTANT
    if (!assistant)
    {
        assistant = new QProcess;
    }

    if (assistant->state() != QProcess::Running)
    {
        QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
#if !defined(Q_OS_MAC)
        app += QLatin1String("assistant");
#else
        app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

        QStringList args;
        args << QLatin1String("-collectionFile")
             << QLatin1String("QMentat.qhc")
             << QLatin1String("-enableRemoteControl");
        assistant->start(app, args);

        if (!assistant->waitForStarted()) {
            QMessageBox::critical(0, QObject::tr("QMentat"),
                                  QObject::tr("Unable to launch Qt Assistant (%1)").arg(app));
        }
    } else {
        QByteArray ba;
         ba.append("setSource qthelp://mike.dusseault.QMentat/doc/index.html\n");
         assistant->write(ba);
    }
#endif
}

void MainWindow::on_actionPreferences_triggered()
{
    PreferencesDialog prefs;
    prefs.exec();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::preferencesChanged()
{
    if (Preferences::getInstance().getAnswerFont() != ui->lineEdit->font())
    {
        ui->lineEdit->setFont(Preferences::getInstance().getAnswerFont());
    }
}
