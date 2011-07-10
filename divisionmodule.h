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

    bool applyConfig();

    // For fixed point results
    void setSettings(BigFixedPoint newFirstMin,
                     BigFixedPoint newFirstMax,
                     BigFixedPoint newLastMin,
                     BigFixedPoint newLastMax,
                     bool newLargestNumberFirst, bool newIntsOnly,
                     int newDecimals, bool newRoundingMode);

    // For integer results only mode
    void setSettings(qint64 newFirstMin, qint64 newFirstMax,
                     qint64 newLastMin, qint64 newLastMax,
                     bool newlargestNumberFirst, bool newIntsOnly,
                     int newDecimals, bool newRoundingMode);

    bool isIntegersOnly() const
    {
        return integersOnly;
    }

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
