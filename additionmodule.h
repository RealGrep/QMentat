#ifndef ADDITIONMODULE_H
#define ADDITIONMODULE_H

#include "practicemodule.h"

#include "bigfixedpoint.h"

// Forward declarations
template <class inttype> class RandomInt;
class AdditionConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class AdditionModule : public PracticeModule
{
public:
    AdditionModule(MainWindow *mw);
    ~AdditionModule();

    QString question();
    bool isCorrect(QString& answerGiven);
    AdditionConfigFrame *configFrame;
    QFrame *getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay *getDisplayFrame();
    QString getAnswerString();

    bool applyConfig();

    void setSettings(BigFixedPoint newFirstMin,
                     BigFixedPoint newFirstMax,
                     BigFixedPoint newLastMin,
                     BigFixedPoint newLastMax,
                     bool newLargestNumberFirst);

private:
    MainWindow *mainWindow;

    BigFixedPoint firstMin;
    BigFixedPoint firstMax;
    BigFixedPoint lastMin;
    BigFixedPoint lastMax;
    BigFixedPoint firstNumber;
    BigFixedPoint lastNumber;
    BigFixedPoint answer;
    bool largestNumberFirst;
};

#endif // ADDITIONMODULE_H
