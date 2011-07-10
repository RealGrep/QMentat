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

    bool applyConfig();

    void setSettings(BigFixedPoint newFirstMin,
                     BigFixedPoint newFirstMax,
                     BigFixedPoint newLastMin,
                     BigFixedPoint newLastMax,
                     bool newLargestNumberFirst,
                     int newDecimals,
                     bool newRoundingMode);

private:
    MainWindow *mainWindow;

    BigFixedPoint firstMin;
    BigFixedPoint firstMax;
    BigFixedPoint lastMin;
    BigFixedPoint lastMax;
    BigFixedPoint firstNumber;
    BigFixedPoint lastNumber;
    BigFixedPoint answer;
    int decimalPlaces;
    bool roundingMode;
    bool largestNumberFirst;
};

#endif // MULTIPLICATIONMODULE_H
