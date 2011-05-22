#ifndef ADDITIONMODULE_H
#define ADDITIONMODULE_H

#include "practicemodule.h"

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

    void setFirstMaximum(quint64 newMax);
    void setFirstMinimum(quint64 newMin);
    void setLastMaximum(quint64 newMax);
    void setLastMinimum(quint64 newMin);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(quint32 newDecimals);

private:
    MainWindow *mainWindow;

    quint64 firstMin;
    quint64 firstMax;
    quint64 lastMin;
    quint64 lastMax;
    quint64 firstNumber;
    quint64 lastNumber;
    quint64 answer;
    bool largestNumberFirst;
    quint32 decimalPlaces;

    RandomInt<quint64> *genFirst;
    RandomInt<quint64> *genLast;

    void firstRangeUpdated();
    void lastRangeUpdated();
};

#endif // ADDITIONMODULE_H
