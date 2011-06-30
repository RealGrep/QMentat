#ifndef DIVISIONMODULE_H
#define DIVISIONMODULE_H

#include "practicemodule.h"

#include <vector>

// Forward declarations
template <class inttype> class RandomInt;
class DivisionConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class DivisionModule : public PracticeModule
{
public:
    DivisionModule(MainWindow *mw);
    ~DivisionModule();

    QString question();
    bool isCorrect(QString& answerGiven);
    DivisionConfigFrame *configFrame;
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

    bool isRangeOk(int firstMin, int firstMax, int lastMin, int lastMax);
    std::vector<unsigned long> *getDivisors(unsigned long num,
                                            unsigned long min,
                                            unsigned long max);
};

#endif // DIVISIONMODULE_H
