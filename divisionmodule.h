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

    bool isIntegersOnly() const
    {
        return integersOnly;
    }

    // For Integer Results Only mode
    void setFirstMaximum(qint64 newMax);
    void setFirstMinimum(qint64 newMin);
    void setLastMaximum(qint64 newMax);
    void setLastMinimum(qint64 newMin);
private:
    MainWindow *mainWindow;

    // In BigFixedPoint mode (not Integer Results Only)
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

    // In Integer Results Only mode
    qint64 firstMinIR;
    qint64 firstMaxIR;
    qint64 lastMinIR;
    qint64 lastMaxIR;
    qint64 firstNumberIR;
    qint64 lastNumberIR;
    qint64 answerIR;

    RandomInt<qint64> *genFirst;

    void firstRangeUpdated();

    //std::vector<BigFixedPoint> *getDivisors(BigFixedPoint& num,
    //                                        BigFixedPoint& min,
    //                                        BigFixedPoint& max);


    bool isRangeOk(qint64 firstMin, qint64 firstMax, qint64 lastMin, qint64 lastMax);
    std::vector<qint64> *getDivisors(qint64 num, qint64 min, qint64 max);
};

#endif // DIVISIONMODULE_H
