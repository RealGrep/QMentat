#ifndef ROOTSMODULE_H
#define ROOTSMODULE_H

#include "rootsconfigframe.h"
#include "practicemodule.h"
#include <QString>
#include <QFrame>
#include "questiondisplay.h"

class RootsConfigFrame;
class MainWindow;

class RootsModule : public PracticeModule
{
public:
    RootsModule(MainWindow *mw);
    ~RootsModule();

    MainWindow *mainWindow;

    int min;
    int max;
    int rootMin;
    int rootMax;
    int firstNumber;
    int root;
    int answer;

    QString question();
    bool isCorrect(QString& answerGiven);
    RootsConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    void setMaximum(int newMax);
    void setMinimum(int newMin);
    void setRootMaximum(int newMax);
    void setRootMinimum(int newMin);
};

#endif // ROOTSMODULE_H
