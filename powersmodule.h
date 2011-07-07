#ifndef POWERSMODULE_H
#define POWERSMODULE_H

#include "practicemodule.h"

#include "bigfixedpoint.h"

// Forward declarations
template <class inttype> class RandomInt;
class PowersConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class PowersModule : public PracticeModule
{
public:
    PowersModule(MainWindow *mw);
    ~PowersModule();

    QString question();
    bool isCorrect(QString& answerGiven);
    PowersConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    void setMaximum(BigFixedPoint newMax);
    void setMinimum(BigFixedPoint newMin);
    void setPowerMaximum(int newMax);
    void setPowerMinimum(int newMin);
    void setDecimalPlaces(int newDecimals);
    void setRoundingMode(bool rnd);

private:
    MainWindow *mainWindow;

    BigFixedPoint min;
    BigFixedPoint max;
    int powerMin;
    int powerMax;
    BigFixedPoint firstNumber;
    int power;
    BigFixedPoint answer;
    int decimalPlaces;
    bool roundingMode;

    RandomInt<int> *genPower;

    void powerRangeUpdated();
};

#endif // POWERSMODULE_H
