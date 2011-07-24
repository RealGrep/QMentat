#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "additionmodule.h"
#include "multiplicationmodule.h"
#include <QMainWindow>
#include "practicemodule.h"

class PracticeModule;
class QProcess;

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
    //void testSQL();
    static quint64 getSeed();

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

    int currentTab;

    QProcess *assistant;

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
    void on_actionContents_triggered();
};

#endif // MAINWINDOW_H

