#ifndef POWERSMODULE_H
#define POWERSMODULE_H

#include "practicemodule.h"
#include "mainwindow.h"
#include "questiondisplay.h"

class PowersConfigFrame;
class MainWindow;

class PowersModule : public PracticeModule
{
public:
    PowersModule(MainWindow *mw);
    ~PowersModule();

    MainWindow *mainWindow;

    int min;
    int max;
    int powerMin;
    int powerMax;
    int firstNumber;
    int power;
    int answer;

    QString question();
    bool isCorrect(QString& answerGiven);
    PowersConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    void setMaximum(int newMax);
    void setMinimum(int newMin);
    void setPowerMaximum(int newMax);
    void setPowerMinimum(int newMin);
};


#endif // POWERSMODULE_H
