#include "rootsmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <QtGui>
#include <string>
#include <QString>
#include "rootsconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
//#include "questiondisplayform.h"
#include "mathdisplayform.h"
#include "random.h"

RootsModule::RootsModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    // Init sane defaults
    firstNumber = 0;
    root = 0;
    answer = 0;

    genFirst = 0;
    genRoot = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("rootsmodule");
    min = settings.value("min", 2).toULongLong();
    max = settings.value("max", 20).toULongLong();
    rootMin = settings.value("rootmin", 2).toULongLong();
    rootMax = settings.value("rootmax", 2).toULongLong();
    decimalPlaces = settings.value("decimalplaces", 0).toUInt();
    settings.endGroup();

    // Make config frame
    configFrame = new RootsConfigFrame();
    configFrame->setModule(this);
    configFrame->setMinimum(this->min);
    configFrame->setMaximum(this->max);
    configFrame->setRootMinimum(this->rootMin);
    configFrame->setRootMaximum(this->rootMax);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());

    firstRangeUpdated();
    rootRangeUpdated();
}

RootsModule::~RootsModule()
{
    assert(configFrame != 0);
    assert(displayFrame != 0);

    delete genFirst;
    genFirst = 0;

    delete genRoot;
    genRoot = 0;

    this->mainWindow->layout()->removeWidget(configFrame);
    configFrame->close();
    delete configFrame;
    configFrame = 0;

    delete displayFrame;
    displayFrame = 0;
}

QFrame* RootsModule::getConfigFrame()
{
    return configFrame;
}

QuestionDisplay* RootsModule::getDisplayFrame()
{
    return displayFrame;
}

QString RootsModule::question()
{

    assert(genFirst != 0);
    assert(genRoot != 0);

    bool bIntegerResults = true;

    firstNumber = (*genFirst)();
    root = (*genRoot)();

    if (bIntegerResults) {
        int maxNum = pow(max, 1.0f/(float)root);
        int minNum = std::max(pow(min, 1.0f/(float)root), (double)min);
        int desiredResult = (qrand() % (maxNum - minNum + 1)) + minNum;
        this->firstNumber = pow(desiredResult, root);
    }

    answer = static_cast<qint64>(pow(firstNumber, 1.0f/(float)root) + 0.0000001);

    QString q = QString("<math><mroot><mi>%1</mi><mn>%2</mn></mroot></math>\n")
                .arg(decimalize(firstNumber, decimalPlaces))
                .arg(decimalize(root, decimalPlaces));

    return q;
}

bool RootsModule::isCorrect(QString& answerGiven)
{
    //! \todo Handle decimal places
    quint64 answerNum = answerGiven.toULongLong();
    if (answerNum == this->answer)
    {
        return true;
    } else {
        return false;
    }
}

QString RootsModule::getAnswerString()
{
    return QString("%1^(1/%2) = %3")
            .arg(decimalize(firstNumber, decimalPlaces))
            .arg(decimalize(root, decimalPlaces))
            .arg(decimalize(answer, decimalPlaces/2));
}

/*! Range of first number updated, so make a new PRG for it.
 * \todo Perhaps this should be generalized so the lastRangeUpdated function
 * doesn't just do the same with different data.
 */
void RootsModule::firstRangeUpdated()
{
    // Get rid of previous generators
    delete genFirst;
    genFirst = 0;

    // Make new generator
    quint64 minGen = min * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 maxGen = max * static_cast<quint64>(pow(10, decimalPlaces));

    genFirst = new RandomInt<quint64>(minGen, maxGen);
}

/*! Range of last number updated, so make a new PRG for it.
 */
void RootsModule::rootRangeUpdated()
{
    // Get rid of previous generator
    delete genRoot;
    genRoot = 0;

    // Make new generator
    quint64 minGen = rootMin * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 maxGen = rootMax * static_cast<quint64>(pow(10, decimalPlaces));

    genRoot = new RandomInt<quint64>(minGen, maxGen);
}

void RootsModule::setMaximum(quint64 newMax)
{
    if (this->max != newMax) {
        this->max = newMax;
        QSettings settings;
        settings.setValue("rootsmodule/max", max);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void RootsModule::setMinimum(quint64 newMin)
{
    if (this->min != newMin) {
        this->min = newMin;
        QSettings settings;
        settings.setValue("rootsmodule/min", min);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void RootsModule::setRootMaximum(quint32 newMax)
{
    if (this->rootMax != newMax) {
        this->rootMax = newMax;
        QSettings settings;
        settings.setValue("rootsmodule/rootmax", rootMax);

        rootRangeUpdated();
        mainWindow->newQuestion();
    }
}

void RootsModule::setRootMinimum(quint32 newMin)
{
    if (this->rootMin != newMin) {
        this->rootMin = newMin;
        QSettings settings;
        settings.setValue("rootsmodule/rootmin", rootMin);

        rootRangeUpdated();
        mainWindow->newQuestion();
    }
}
void RootsModule::setDecimalPlaces(quint32 newDecimals)
{
    if (decimalPlaces != newDecimals) {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("rootsmodule/decimalplaces", decimalPlaces);

        firstRangeUpdated();
        rootRangeUpdated();
        mainWindow->newQuestion();
    }
}
