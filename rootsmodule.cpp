/* Copyright (c) 2013 Mike Dusseault
 *
 * This file is part of QMentat.
 *
 * QMentat is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QMentat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QMentat.  If not, see <http://www.gnu.org/licenses/>.
 */
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

#ifdef USE_MATH_DISPLAY
#   include "mathdisplayform.h"
#else
#   include "questiondisplayform.h"
#endif

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
    max = BigFixedPoint(settings.value("max", 100).toString());
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
#ifdef USE_MATH_DISPLAY
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());
#else
    displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());
#endif

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

bool RootsModule::isRangeOk(const BigFixedPoint& newMin, const BigFixedPoint& newMax,
                            int newRootMin, int newRootMax)
{
    bool isOk = false;

    for (int i = newRootMin; i <= newRootMax; ++i)
    {
        BigFixedPoint maxNum = newMax.root(i);
        BigFixedPoint minNum = BigFixedPoint::max(newMin.root(i), newMin);
        if ((maxNum - minNum) >= 0)
        {
            isOk = true;
            break;
        }
    }

    return isOk;
}

QString RootsModule::question()
{
    assert(genRoot != 0);

    if (integersOnly) {
        BigFixedPoint maxNum;
        BigFixedPoint minNum;
        // Iterate until we get a root that leads to a usable range.
        do {
            root = (*genRoot)();

            maxNum = max.root(root);
            minNum = BigFixedPoint::max(min.root(root), min);
        } while ((maxNum - minNum) < 0);

        BigFixedPoint desiredResult = BigFixedPoint::random(minNum, maxNum);

        firstNumber = desiredResult.pow(root);
        answer = desiredResult;
        answer.scale(0);
    } else {
        root = (*genRoot)();

        firstNumber = BigFixedPoint::random(min, max);
        BigFixedPoint firstNumberScaled = firstNumber;
        // Add one for rounding purposes
        firstNumberScaled.scale(((decimalPlaces + firstNumber.getDecimalPlaces() + 1) * root));
        answer = firstNumberScaled.root(root);
        answer.scale(decimalPlaces);
    }

#ifdef USE_MATH_DISPLAY
    QString q = QString("<math><mroot><mi>%1</mi><mn>%2</mn></mroot></math>\n")
                .arg(firstNumber.toString())
                .arg(root);
#else
    QString q = QString("%1\n| %2")
                .arg(firstNumber.toString())
                .arg(root);
#endif

    return q;
}

bool RootsModule::isCorrect(const QString& answerGiven)
{
    BigFixedPoint answerNum(answerGiven);
    answerNum.scale(answer.getDecimalPlaces());

    if (answerNum == answer)
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

void RootsModule::setSettings(const BigFixedPoint& newFirstMin,
                              const BigFixedPoint& newFirstMax,
                              int newRootMin,
                              int newRootMax,
                              bool newIntsOnly,
                              int newDecimals,
                              bool newRoundingMode)
{
    bool settingsChanged = false;

    // RANGE
    if ((max != newFirstMax)
     || (max.getDecimalPlaces() != newFirstMax.getDecimalPlaces()))
    {
        max = newFirstMax;
        QSettings settings;
        settings.setValue("rootsmodule/max", max.toString());
        settingsChanged = true;
    }

    if ((min != newFirstMin)
     || (min.getDecimalPlaces() != newFirstMin.getDecimalPlaces()))
    {
        min = newFirstMin;
        QSettings settings;
        settings.setValue("rootsmodule/min", min.toString());
        settingsChanged = true;
    }

    if (rootMax != newRootMax)
    {
        rootMax = newRootMax;
        QSettings settings;
        settings.setValue("rootsmodule/rootmax", rootMax);
        settingsChanged = true;
    }

    if (rootMin != newRootMin)
    {
        rootMin = newRootMin;
        QSettings settings;
        settings.setValue("rootsmodule/rootmin", rootMin);
        settingsChanged = true;
    }

    // RESULTS
    if (decimalPlaces != newDecimals)
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("rootsmodule/decimalplaces", decimalPlaces);
        settingsChanged = true;
    }

    if (roundingMode != newRoundingMode)
    {
        roundingMode = newRoundingMode;
        BigFixedPoint::setRounding(roundingMode);
        QSettings settings;
        settings.setValue("rootsmodule/roundingmode", roundingMode);
        settingsChanged = true;
    }

    if (integersOnly != newIntsOnly)
    {
        integersOnly = newIntsOnly;
        QSettings settings;
        settings.setValue("rootsmodule/integersonly", integersOnly);
        settingsChanged = true;
    }

    if (settingsChanged)
    {
        rootRangeUpdated();
        mainWindow->newQuestion();
    }
}

bool RootsModule::applyConfig()
{
    if (configFrame != 0)
    {
        return configFrame->applyConfig();
    }

    return false;
}
