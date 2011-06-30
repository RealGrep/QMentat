#ifndef SUBTRACTIONMODULE_H
#define SUBTRACTIONMODULE_H

#include "practicemodule.h"

// Forward declarations
template <class inttype> class RandomInt;
class SubtractionConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class SubtractionModule : public PracticeModule
{
public:
    SubtractionModule(MainWindow *mw);
    ~SubtractionModule();

    QString question();
    bool isCorrect(QString& answerGiven);
    SubtractionConfigFrame *configFrame;
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
    qint64 answer;
    bool largestNumberFirst;
    quint32 decimalPlaces;

    RandomInt<quint64> *genFirst;
    RandomInt<quint64> *genLast;

    void firstRangeUpdated();
    void lastRangeUpdated();
};

#endif // SUBTRACTIONMODULE_H
