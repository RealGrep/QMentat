#include "rootsmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <QtGui>
#include <string>
#include <QString>
#include "rootsconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
//#include "questiondisplayform.h"R
#include "mathdisplayform.h"
#include "random.h"
#include "bigfixedpoint.h"

RootsModule::RootsModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    // Init sane defaults
    root = 0;

    genRoot = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("rootsmodule");
    roundingMode = settings.value("roundingmode", false).toBool();
    BigFixedPoint::setRounding(roundingMode == 1);
    min = BigFixedPoint(settings.value("min", 2).toString());
    max = BigFixedPoint(settings.value("max", 20).toString());
    rootMin = settings.value("rootmin", 2).toInt();
    rootMax = settings.value("rootmax", 2).toInt();
    decimalPlaces = settings.value("decimalplaces", 0).toInt();
    integersOnly = settings.value("integersonly", true).toBool();
    settings.endGroup();

    // Make config frame
    configFrame = new RootsConfigFrame();
    configFrame->setModule(this);
    configFrame->setMinimum(min.toString());
    configFrame->setMaximum(max.toString());
    configFrame->setRootMinimum(rootMin);
    configFrame->setRootMaximum(rootMax);
    configFrame->setDecimalPlaces(decimalPlaces);
    configFrame->setIntegersOnly(integersOnly);
    configFrame->setRoundingMode((roundingMode == true) ? 1 : 0);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());

    rootRangeUpdated();
}

RootsModule::~RootsModule()
{
    assert(configFrame != 0);
    assert(displayFrame != 0);

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
    assert(genRoot != 0);

    root = (*genRoot)();

    if (integersOnly) {
        BigFixedPoint maxNum = max.root(root);
        BigFixedPoint minNum = BigFixedPoint::max(min.root(root), min);
        BigFixedPoint desiredResult = BigFixedPoint::random(minNum, maxNum);

        firstNumber = desiredResult.pow(root);
        answer = desiredResult;
        answer.scale(0);
    } else {
        firstNumber = BigFixedPoint::random(min, max);
        BigFixedPoint firstNumberScaled = firstNumber;
        // Add one for rounding purposes
        firstNumberScaled.scale(((decimalPlaces + firstNumber.getDecimalPlaces() + 1) * root));
        answer = firstNumberScaled.root(root);
        answer.scale(decimalPlaces);
    }

    QString q = QString("<math><mroot><mi>%1</mi><mn>%2</mn></mroot></math>\n")
                .arg(firstNumber.toString())
                .arg(root);

    return q;
}

bool RootsModule::isCorrect(QString& answerGiven)
{
    BigFixedPoint answerNum(answerGiven);
    answerNum.scale(answer.getDecimalPlaces());

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
            .arg(firstNumber.toString())
            .arg(root)
            .arg(answer.toString());
}

/*! Range of root updated, so make a new PRG for it.
 */
void RootsModule::rootRangeUpdated()
{
    assert(rootMin > 0);
    assert(rootMax > 0);

    // Get rid of previous generator
    delete genRoot;
    genRoot = 0;

    genRoot = new RandomInt<int>(rootMin, rootMax);
}

void RootsModule::setMaximum(BigFixedPoint newMax)
{
    if ((max != newMax)
        || (max.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        max = newMax;
        QSettings settings;
        settings.setValue("rootsmodule/max", max.toString());

        mainWindow->newQuestion();
    }
}

void RootsModule::setMinimum(BigFixedPoint newMin)
{
    if ((min != newMin)
        || (min.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        min = newMin;
        QSettings settings;
        settings.setValue("rootsmodule/min", min.toString());

        mainWindow->newQuestion();
    }
}

void RootsModule::setRootMaximum(int newMax)
{
    if (this->rootMax != newMax)
    {
        rootMax = newMax;
        QSettings settings;
        settings.setValue("rootsmodule/rootmax", rootMax);

        rootRangeUpdated();
        mainWindow->newQuestion();
    }
}

void RootsModule::setRootMinimum(int newMin)
{
    if (this->rootMin != newMin)
    {
        this->rootMin = newMin;
        QSettings settings;
        settings.setValue("rootsmodule/rootmin", rootMin);

        rootRangeUpdated();
        mainWindow->newQuestion();
    }
}
void RootsModule::setDecimalPlaces(int newDecimals)
{
    if (decimalPlaces != newDecimals)
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("rootsmodule/decimalplaces", decimalPlaces);

        //rootRangeUpdated();
        mainWindow->newQuestion();
    }
}

void RootsModule::setRoundingMode(bool rnd)
{
    if (roundingMode != rnd)
    {
        roundingMode = rnd;
        BigFixedPoint::setRounding(roundingMode);
        QSettings settings;
        settings.setValue("rootsmodule/roundingmode", roundingMode);

        mainWindow->newQuestion();
    }
}

void RootsModule::setIntegersOnly(bool intsOnly)
{
    if (integersOnly != intsOnly)
    {
        integersOnly = intsOnly;
        QSettings settings;
        settings.setValue("rootsmodule/integersonly", integersOnly);

        mainWindow->newQuestion();
    }
}
