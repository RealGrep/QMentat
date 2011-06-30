#ifndef POWERSMODULE_H
#define POWERSMODULE_H

#include "practicemodule.h"

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

    void setMaximum(quint64 newMax);
    void setMinimum(quint64 newMin);
    void setPowerMaximum(quint32 newMax);
    void setPowerMinimum(quint32 newMin);
    void setDecimalPlaces(quint32 newDecimals);

private:
    MainWindow *mainWindow;

    quint64 min;
    quint64 max;
    quint32 powerMin;
    quint32 powerMax;
    quint64 firstNumber;
    quint32 power;
    quint64 answer;
    quint32 decimalPlaces;

    RandomInt<quint64> *genFirst;
    RandomInt<quint64> *genPower;

    void firstRangeUpdated();
    void powerRangeUpdated();
};

#endif // POWERSMODULE_H
