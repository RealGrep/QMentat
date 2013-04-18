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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "additionmodule.h"
#include "multiplicationmodule.h"
#include <QMainWindow>
#include "practicemodule.h"
#include "preferenceslistener.h"

class PracticeModule;
#ifdef USE_ASSISTANT
class QProcess;
#else
class QHelpEngine;
class HelpBrowser;
#endif

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public PreferencesListener
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

#ifdef USE_ASSISTANT
    QProcess *assistant;
#else
    QHelpEngine *helpEngine;
    //QTextBrowser *helpBrowser;
    HelpBrowser *helpBrowser;
#endif

    void preferencesChanged();

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
    void on_actionPreferences_triggered();
    void on_actionAbout_Qt_triggered();

    //void setSource(const QUrl &);
};

#endif // MAINWINDOW_H

