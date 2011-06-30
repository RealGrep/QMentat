#ifndef ROOTSMODULE_H
#define ROOTSMODULE_H

#include "practicemodule.h"

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

    void setMaximum(quint64 newMax);
    void setMinimum(quint64 newMin);
    void setRootMaximum(quint32 newMax);
    void setRootMinimum(quint32 newMin);
    void setDecimalPlaces(quint32 newDecimals);

private:
    MainWindow *mainWindow;

    quint64 min;
    quint64 max;
    quint32 rootMin;
    quint32 rootMax;
    quint64 firstNumber;
    quint32 root;
    quint64 answer;
    quint32 decimalPlaces;

    RandomInt<quint64> *genFirst;
    RandomInt<quint64> *genRoot;

    void firstRangeUpdated();
    void rootRangeUpdated();
};

#endif // ROOTSMODULE_H
