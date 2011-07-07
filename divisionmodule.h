#ifndef DIVISIONMODULE_H
#define DIVISIONMODULE_H

#include "practicemodule.h"
#include "bigfixedpoint.h"

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

    void setFirstMaximum(BigFixedPoint newMax);
    void setFirstMinimum(BigFixedPoint newMin);
    void setLastMaximum(BigFixedPoint newMax);
    void setLastMinimum(BigFixedPoint newMin);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(int newDecimals);
    void setRoundingMode(bool rnd);
    void setIntegersOnly(bool intsOnly);

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
    //! \todo Enum this. 0 = truncate, 0 = round
    bool roundingMode;
    bool integersOnly;

    std::vector<BigFixedPoint> *getDivisors(BigFixedPoint& num,
                                            BigFixedPoint& min,
                                            BigFixedPoint& max);

#if 0
    bool isRangeOk(int firstMin, int firstMax, int lastMin, int lastMax);
    std::vector<unsigned long> *getDivisors(unsigned long num,
                                            unsigned long min,
                                            unsigned long max);
#endif
};

#endif // DIVISIONMODULE_H
