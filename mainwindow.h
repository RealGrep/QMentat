#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "additionmodule.h"
#include "multiplicationmodule.h"
#include <QMainWindow>
#include "practicemodule.h"

class PracticeModule;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void newQuestion();
    //! \todo Test method for SQL, remove when done
    void testSQL();

protected:
    int answerGiven;
    int totalQuestions;
    int totalCorrect;
    int totalWrong;

    PracticeModule* module;
private:
    Ui::MainWindow *ui;

    void readSettings();
    void writeSettings();

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_actionStatistics_triggered();
    void moduleChange(PracticeModule *module);
    void on_actionRoots_triggered();
    void on_actionPowers_triggered();
    void on_actionAbout_triggered();
    void on_actionDivision_triggered();
    void on_actionSubtraction_triggered();
    void on_actionMultiplication_triggered();
    void on_actionAddition_triggered();
    void on_lineEdit_returnPressed();
};

#endif // MAINWINDOW_H

