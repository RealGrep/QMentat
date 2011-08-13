#ifndef ROOTSMODULE_H
#define ROOTSMODULE_H

#include "practicemodule.h"

#include "bigfixedpoint.h"

// Forward declarations
template <class inttype> class RandomInt;
class RootsConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class RootsModule : public PracticeModule
{
public:
    RootsModule(MainWindow *mw);
    ~RootsModule();

    QString question();
    bool isCorrect(QString& answerGiven);
    RootsConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    bool isRangeOk(BigFixedPoint newMin, BigFixedPoint newMax,
                   int newRootMin, int newRootMax);

    bool applyConfig();

    void setSettings(BigFixedPoint newFirstMin,
                     BigFixedPoint newFirstMax,
                     int newRootMin,
                     int newRootMax,
                     bool newIntsOnly,
                     int newDecimals,
                     bool newRoundingMode);

private:
    MainWindow *mainWindow;

    BigFixedPoint min;
    BigFixedPoint max;
    int rootMin;
    int rootMax;
    BigFixedPoint firstNumber;
    int root;
    BigFixedPoint answer;
    int decimalPlaces;
    bool roundingMode;
    bool integersOnly;

    RandomInt<int> *genRoot;

    void rootRangeUpdated();
};

#endif // ROOTSMODULE_H
