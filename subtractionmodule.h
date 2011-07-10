#ifndef SUBTRACTIONMODULE_H
#define SUBTRACTIONMODULE_H

#include "practicemodule.h"
#include "bigfixedpoint.h"

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

    bool applyConfig();

    void setSettings(BigFixedPoint newFirstMin,
                     BigFixedPoint newFirstMax,
                     BigFixedPoint newLastMin,
                     BigFixedPoint newLastMax,
                     bool newLargestNumberFirst);

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
};

#endif // SUBTRACTIONMODULE_H
