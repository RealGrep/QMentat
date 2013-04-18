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
#include "multiplicationmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <cassert>
#include <QtGui>
#include <string>
#include <QString>
#include "multiplicationconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "questiondisplayform.h"
#include "random.h"
#include "bigfixedpoint.h"

MultiplicationModule::MultiplicationModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    // Read config
    QSettings settings;
    settings.beginGroup("multiplicationmodule");
    roundingMode = settings.value("roundingmode", false).toBool();
    BigFixedPoint::setRounding(roundingMode == 1);
    firstMin = BigFixedPoint(settings.value("firstmin", 2).toString());
    firstMax = BigFixedPoint(settings.value("firstmax", 10).toString());
    lastMin = BigFixedPoint(settings.value("lastmin", 2).toString());
    lastMax = BigFixedPoint(settings.value("lastmax", 10).toString());
    largestNumberFirst = settings.value("largestNumberFirst", false).toBool();
    decimalPlaces = settings.value("decimalplaces", 0).toInt();
    settings.endGroup();

    // Make config frame
    configFrame = new MultiplicationConfigFrame();
    configFrame->setModule(this);
    configFrame->setFirstMinimum(firstMin.toString());
    configFrame->setFirstMaximum(firstMax.toString());
    configFrame->setLastMinimum(lastMin.toString());
    configFrame->setLastMaximum(lastMax.toString());
    configFrame->setLargestNumberFirst(largestNumberFirst);
    configFrame->setDecimalPlaces(decimalPlaces);
    configFrame->setRoundingMode((roundingMode == true) ? 1 : 0);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());
}

MultiplicationModule::~MultiplicationModule()
{
    assert(configFrame != 0);
    assert(displayFrame != 0);

    this->mainWindow->layout()->removeWidget(configFrame);
    configFrame->close();
    delete configFrame;
    configFrame = 0;

    delete displayFrame;
    displayFrame = 0;
}

QFrame* MultiplicationModule::getConfigFrame()
{
    return configFrame;
}

QuestionDisplay* MultiplicationModule::getDisplayFrame()
{
    return displayFrame;
}

QString MultiplicationModule::question()
{
    firstNumber = BigFixedPoint::random(firstMin, firstMax);
    lastNumber = BigFixedPoint::random(lastMin, lastMax);

    // Swap largest number first, if necessary and set to do so
    if (largestNumberFirst && (lastNumber > firstNumber))
    {
        BigFixedPoint tmp = firstNumber;
        firstNumber = lastNumber;
        lastNumber = tmp;
    }

    // Calculate answer
    answer = firstNumber * lastNumber;
    assert(decimalPlaces >= 0);
    answer.scale(decimalPlaces);

    int decimals = std::max(firstNumber.getDecimalPlaces(), lastNumber.getDecimalPlaces());
    BigFixedPoint firstDisplay(firstNumber);
    firstDisplay.scale(decimals);
    BigFixedPoint lastDisplay(lastNumber);
    lastDisplay.scale(decimals);

    // Build question string
    QString q = QString("%1\nx %2")
                .arg(firstDisplay.toString())
                .arg(lastDisplay.toString());

    return q;
}

bool MultiplicationModule::isCorrect(QString& answerGiven)
{
    BigFixedPoint answerNum(answerGiven);
    answerNum.scale(answer.getDecimalPlaces());

    //qDebug() << "isCorrect: answerGiven = " << answerNum.toString() << "; answer = " << answer.toString();
    if (answerNum == answer)
    {
        return true;
    } else {
        return false;
    }
}

QString MultiplicationModule::getAnswerString()
{
    return QString("%1 x %2 = %3")
            .arg(firstNumber.toString())
            .arg(lastNumber.toString())
            .arg(answer.toString());
}

void MultiplicationModule::setSettings(BigFixedPoint newFirstMin,
                                       BigFixedPoint newFirstMax,
                                       BigFixedPoint newLastMin,
                                       BigFixedPoint newLastMax,
                                       bool newLargestNumberFirst,
                                       int newDecimals,
                                       bool newRoundingMode)
{
    bool settingsChanged = false;

    // RANGE
    if ((firstMax != newFirstMax)
     || (firstMax.getDecimalPlaces() != newFirstMax.getDecimalPlaces()))
    {
        firstMax = newFirstMax;
        QSettings settings;
        settings.setValue("multiplicationmodule/firstmax", firstMax.toString());
        settingsChanged = true;
    }

    if ((firstMin != newFirstMin)
     || (firstMin.getDecimalPlaces() != newFirstMin.getDecimalPlaces()))
    {
        firstMin = newFirstMin;
        QSettings settings;
        settings.setValue("multiplicationmodule/firstmin", firstMin.toString());
        settingsChanged = true;
    }

    if ((lastMax != newLastMax)
     || (lastMax.getDecimalPlaces() != newLastMax.getDecimalPlaces()))
    {
        lastMax = newLastMax;
        QSettings settings;
        settings.setValue("multiplicationmodule/lastmax", lastMax.toString());
        settingsChanged = true;
    }

    if ((lastMin != newLastMin)
     || (lastMin.getDecimalPlaces() != newLastMin.getDecimalPlaces()))
    {
        lastMin = newLastMin;
        QSettings settings;
        settings.setValue("multiplicationmodule/lastmin", lastMin.toString());
        settingsChanged = true;
    }

    // RESULTS
    if (this->largestNumberFirst != newLargestNumberFirst)
    {
        this->largestNumberFirst = newLargestNumberFirst;
        QSettings settings;
        settings.setValue("multiplicationmodule/largestNumberFirst", largestNumberFirst);
        settingsChanged = true;
    }

    if (decimalPlaces != newDecimals)
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("multiplicationmodule/decimalplaces", decimalPlaces);
        settingsChanged = true;
    }

    if (roundingMode != newRoundingMode)
    {
        roundingMode = newRoundingMode;
        BigFixedPoint::setRounding(roundingMode);
        QSettings settings;
        settings.setValue("multiplicationmodule/roundingmode", roundingMode);
        settingsChanged = true;
    }

    if (settingsChanged)
    {
        mainWindow->newQuestion();
    }
}

bool MultiplicationModule::applyConfig()
{
    if (configFrame != 0)
    {
        return configFrame->applyConfig();
    }

    return false;
}
