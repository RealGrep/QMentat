#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <cassert>
#include <QtGui>
#include "practicemodule.h"
#include "additionmodule.h"
#include "subtractionmodule.h"
#include "multiplicationmodule.h"
#include "divisionmodule.h"
#include "powersmodule.h"
#include "rootsmodule.h"
#include "questiondisplayform.h"
#include <QtSql/QtSql>
#include <QDebug>
#include <QtMmlWidget>

#include "statisticsdialog.h"


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
    QCoreApplication::setOrganizationName("mike.dusseault");
    QCoreApplication::setApplicationName("QMentat");

    ui->setupUi(this);

    // Initialize sane values
    this->answerGiven = 0;
    this->totalQuestions = 0;
    this->totalCorrect = 0;
    this->totalWrong = 0;

    // Load up the default module
    //! \todo Save/restore currently active module at app exit/load
    this->module = 0;
    moduleChange(new AdditionModule(this));

    ui->centralWidget->layout()->addWidget(this->module->getConfigFrame());

    // Restore saved settings
    readSettings();

    // Kick off first question
    newQuestion();
    this->ui->lineEdit->setFocus();

    /*
    std::cout << std::endl << "=== FixedPoint ===" << std::endl;
    FixedPoint<quint64> fixed1(123456, 3);
    FixedPoint<quint64> fixed2(234567, 4);

    std::cout << fixed1 << std::endl;
    std::cout << "123.456 scaled to 2 = " << fixed1.scale(2) << std::endl;
    std::cout << "123.456 scaled to 4 = " << fixed1.scale(4) << std::endl;

    FixedPoint<quint64> result = fixed1 + fixed2;
    qDebug() << "123.456 + 23.4567 = " << PracticeModule::decimalize(result.getValue(), result.getDecimalPlaces());
    qDebug() << "Result (decimals): " << result.getValue() << " (" << result.getDecimalPlaces() << ")";

    result = fixed1 - fixed2;
    qDebug() << "123.456 - 23.4567 = " << PracticeModule::decimalize(result.getValue(), result.getDecimalPlaces());
    qDebug() << "Result (decimals): " << result.getValue() << " (" << result.getDecimalPlaces() << ")";

    result = fixed1 * fixed2;
    qDebug() << "123.456 * 23.4567 = " << PracticeModule::decimalize(result.getValue(), result.getDecimalPlaces());
    qDebug() << "Result (decimals): " << result.getValue() << " (" << result.getDecimalPlaces() << ")";

    result = fixed1 / fixed2;
    qDebug() << "123.456 / 23.4567 = ";// << PracticeModule::decimalize(result.getValue(), result.getDecimalPlaces());
    qDebug() << "Result (decimals): " << result.getValue() << " (" << result.getDecimalPlaces() << ")";

    qDebug() << "exp10(2) = " << exp10(2);
    */
}

MainWindow::~MainWindow()
{
    writeSettings();

    delete module;
    module = 0;

    delete ui;
}

void MainWindow::testSQL()
{
    /*
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
   */
}

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
}

/*! Writes current Qt settings and saves them.
 */
void MainWindow::writeSettings() {
    // Save postion/size of main window
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("state", saveState());
}


/*! Gets a new question and displays it.
 */
void MainWindow::newQuestion()
{
    assert(module != 0);
    QString q = module->question();

    this->module->getDisplayFrame()->setText(q);
    this->ui->lineEdit->clear();

    //this->ui->lineEdit->setFocus();
}

void MainWindow::on_lineEdit_editingFinished()
{
    //std::cerr << "Answer given: " << ui->lineEdit->text().toInt() << std::endl;
}

/*! Return pressed, so verify the answer given and display result.
 */
void MainWindow::on_lineEdit_returnPressed()
{
    assert(module != 0);

    // Answer as integer
    QString answerGiven = ui->lineEdit->text();
    if (answerGiven.size() == 0)
    {
        answerGiven = "0";
    }

    // Handle correct or not
    if (module->isCorrect(answerGiven)) {
        ui->textEdit->setText(tr("Correct!"));
        this->totalCorrect++;
    } else {
        ui->textEdit->setText(tr("Wrong! %1").arg(module->getAnswerString()));
        this->totalWrong++;
    }
    this->totalQuestions++;
    statusBar()->showMessage(tr( "Total: %1  Correct/Wrong: %2/%3  %4%5 success rate" )
                             .arg(this->totalQuestions )
                             .arg(this->totalCorrect)
                             .arg(this->totalWrong)
                             .arg(((double)totalCorrect / (double)totalQuestions) * 100.0, 0, 'f', 2)
                             .arg(QLocale::system().percent()));

    newQuestion();
    this->ui->lineEdit->setFocus();
}

/*! Method to change the currently active module to another module (addition,
 *    division, etc).
 * \param module The PracticeModule module which will be active, replacing
 *    the old one.
 */
void MainWindow::moduleChange(PracticeModule *module) {
    assert(module != 0);

    // Load the config and display frames
    if (this->module != 0) {
        ui->centralWidget->layout()->removeWidget(this->module->getConfigFrame());
        this->module->getConfigFrame()->close();

        ui->displayPane->layout()->removeWidget(this->module->getDisplayFrame());
        this->module->getDisplayFrame()->close();

        delete this->module;
        this->module = 0;
    }

    // Grab new module and get new config and display frames
    this->module = module;
    ui->centralWidget->layout()->addWidget(this->module->getConfigFrame());
    ui->displayPane->layout()->addWidget(this->module->getDisplayFrame());

    assert(this->module != 0);

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
    QMessageBox::about(this, tr("About QMentat"), tr("QMentat\n\nWritten by Mike Dusseault\nCopyright 2011 Mike Dusseault.\nAll Rights Reserved."));
}

void MainWindow::on_actionStatistics_triggered()
{
    StatisticsDialog stats;
    stats.set(totalCorrect, totalWrong);
    stats.exec();
}
