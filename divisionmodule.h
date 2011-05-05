#ifndef DIVISIONMODULE_H
#define DIVISIONMODULE_H

#include "mainwindow.h"
#include "practicemodule.h"
#include "divisionconfigframe.h"
#include "questiondisplay.h"

#include <vector>

class DivisionConfigFrame;
class MainWindow;

class DivisionModule : public PracticeModule
{
public:
    DivisionModule(MainWindow *mw);
    ~DivisionModule();

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
    DivisionConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    bool isRangeOk(int firstMin, int firstMax, int lastMin, int lastMax);
    std::vector<unsigned long> *getDivisors(unsigned long num,
                                            unsigned long min,
                                            unsigned long max);

    void setFirstMaximum(int newMax);
    void setFirstMinimum(int newMin);
    void setLastMaximum(int newMax);
    void setLastMinimum(int newMin);
    void setLargestNumberFirst(bool b);
};

#endif // DIVISIONMODULE_H
