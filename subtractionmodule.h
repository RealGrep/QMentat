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

    void setFirstMaximum(BigFixedPoint newMax);
    void setFirstMinimum(BigFixedPoint newMin);
    void setLastMaximum(BigFixedPoint newMax);
    void setLastMinimum(BigFixedPoint newMin);
    void setLargestNumberFirst(bool b);

    bool applyConfig();

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
