#ifndef PRACTICEMODULE_H
#define PRACTICEMODULE_H

#include <QtGui>
#include "questiondisplay.h"

class PracticeModule
{
public:
    //PracticeModule(MainWindow *mw);
    virtual QString question() = 0;
    virtual bool isCorrect(QString& answerGiven) = 0;
    virtual QFrame* getConfigFrame() = 0;
    virtual QuestionDisplay* getDisplayFrame() = 0;
    virtual QString getAnswerString() = 0;
};

#endif // PRACTICEMODULE_H
