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

    //QString decimalize(quint64 num, quint32 decimals);

    void setFirstMaximum(BigFixedPoint newMax);
    void setFirstMinimum(BigFixedPoint newMin);
    void setLastMaximum(BigFixedPoint newMax);
    void setLastMinimum(BigFixedPoint newMin);
    void setLargestNumberFirst(bool b);

    bool applyConfig();

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
