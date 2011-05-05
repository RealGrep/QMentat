#ifndef SUBTRACTIONMODULE_H
#define SUBTRACTIONMODULE_H

#include "practicemodule.h"
#include "mainwindow.h"
#include "questiondisplay.h"

class SubtractionConfigFrame;
class MainWindow;

class SubtractionModule : public PracticeModule
{
public:
    SubtractionModule(MainWindow *mw);
    ~SubtractionModule();

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
    SubtractionConfigFrame *configFrame;
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


#endif // SUBTRACTIONMODULE_H
