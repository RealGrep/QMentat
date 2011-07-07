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

    void setMaximum(BigFixedPoint newMax);
    void setMinimum(BigFixedPoint newMin);
    void setRootMaximum(int newMax);
    void setRootMinimum(int newMin);
    void setDecimalPlaces(int newDecimals);
    void setIntegersOnly(bool intsOnly);
    void setRoundingMode(bool rnd);

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
