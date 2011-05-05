#ifndef MULTIPLICATIONMODULE_H
#define MULTIPLICATIONMODULE_H

#include "practicemodule.h"
#include "mainwindow.h"

class MultiplicationConfigFrame;
class MainWindow;

class MultiplicationModule : public PracticeModule
{
public:
    MultiplicationModule(MainWindow *mw);
    ~MultiplicationModule();

    MainWindow *mainWindow;

    int firstMin;
    int firstMax;
    int lastMin;
    int lastMax;
    int firstNumber;
    int lastNumber;
    int answer;
    bool largestNumberFirst;


    QString question();
    bool isCorrect(QString& answerGiven);
    MultiplicationConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    void setFirstMaximum(int newMax);
    void setFirstMinimum(int newMin);
    void setLastMaximum(int newMax);
    void setLastMinimum(int newMin);
    void setLargestNumberFirst(bool b);
};

#endif // MULTIPLICATIONMODULE_H
