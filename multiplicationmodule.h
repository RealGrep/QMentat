#ifndef MULTIPLICATIONMODULE_H
#define MULTIPLICATIONMODULE_H

#include "practicemodule.h"
#include "bigfixedpoint.h"

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

    void setFirstMaximum(BigFixedPoint newMax);
    void setFirstMinimum(BigFixedPoint newMin);
    void setLastMaximum(BigFixedPoint newMax);
    void setLastMinimum(BigFixedPoint newMin);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(int newDecimals);

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
    int decimalPlaces;
};

#endif // MULTIPLICATIONMODULE_H
