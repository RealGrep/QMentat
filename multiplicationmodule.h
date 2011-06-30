#ifndef MULTIPLICATIONMODULE_H
#define MULTIPLICATIONMODULE_H

#include "practicemodule.h"

// Forward declarations
template <class inttype> class RandomInt;
class MultiplicationConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class MultiplicationModule : public PracticeModule
{
public:
    MultiplicationModule(MainWindow *mw);
    ~MultiplicationModule();

    QString question();
    bool isCorrect(QString& answerGiven);
    MultiplicationConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

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

#endif // MULTIPLICATIONMODULE_H
