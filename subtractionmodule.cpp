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
#include "subtractionmodule.h"

#include <QtGui>
//#include <string>
#include <QString>

#include <cassert>

#include <iostream>
#include "subtractionconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "questiondisplayform.h"
#include "bigfixedpoint.h"

SubtractionModule::SubtractionModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    // Read config
    QSettings settings;
    settings.beginGroup("subtractionmodule");
    BigFixedPoint::setRounding(false);
    firstMin = BigFixedPoint(settings.value("firstmin", 2).toString());
    firstMax = BigFixedPoint(settings.value("firstmax", 100).toString());
    lastMin = BigFixedPoint(settings.value("lastmin", 2).toString());
    lastMax = BigFixedPoint(settings.value("lastmax", 100).toString());
    largestNumberFirst = settings.value("largestNumberFirst", true).toBool();
    settings.endGroup();

    // Make config frame
    configFrame = new SubtractionConfigFrame();
    configFrame->setModule(this);
    configFrame->setFirstMinimum(firstMin.toString());
    configFrame->setFirstMaximum(firstMax.toString());
    configFrame->setLastMinimum(lastMin.toString());
    configFrame->setLastMaximum(lastMax.toString());
    configFrame->setLargestNumberFirst(largestNumberFirst);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());
}

SubtractionModule::~SubtractionModule()
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

QFrame* SubtractionModule::getConfigFrame()
{
    return configFrame;
}

QuestionDisplay* SubtractionModule::getDisplayFrame()
{
    return displayFrame;
}

QString SubtractionModule::question()
{

    firstNumber = BigFixedPoint::random(firstMin, firstMax);
    lastNumber = BigFixedPoint::random(lastMin, lastMax);

    // Scale to largest number of decimals
    int maxDecimals = std::max(firstNumber.getDecimalPlaces(), lastNumber.getDecimalPlaces());
    if (firstNumber.getDecimalPlaces() < maxDecimals)
    {
        firstNumber.scale(maxDecimals);
    }
    if (lastNumber.getDecimalPlaces() < maxDecimals)
    {
        lastNumber.scale(maxDecimals);
    }

    // Swap largest number first, if necessary and set to do so
    if (largestNumberFirst && (lastNumber > firstNumber))
    {
        BigFixedPoint tmp = firstNumber;
        firstNumber = lastNumber;
        lastNumber = tmp;
    }

    // Calculate answer
    answer = firstNumber - lastNumber;

    // Build question string
    QString q = QString("%1\n- %2")
                .arg(firstNumber.toString())
                .arg(lastNumber.toString());

    return q;
}

bool SubtractionModule::isCorrect(const QString& answerGiven)
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

QString SubtractionModule::getAnswerString()
{
    return QString("%1 - %2 = %3")
            .arg(firstNumber.toString())
            .arg(lastNumber.toString())
            .arg(answer.toString());
}

void SubtractionModule::setSettings(const BigFixedPoint& newFirstMin,
                                 const BigFixedPoint& newFirstMax,
                                 const BigFixedPoint& newLastMin,
                                 const BigFixedPoint& newLastMax,
                                 bool newLargestNumberFirst)
{
    bool settingsChanged = false;

    // RANGE
    if ((firstMax != newFirstMax)
     || (firstMax.getDecimalPlaces() != newFirstMax.getDecimalPlaces()))
    {
        firstMax = newFirstMax;
        QSettings settings;
        settings.setValue("subtractionmodule/firstmax", firstMax.toString());
        settingsChanged = true;
    }

    if ((firstMin != newFirstMin)
     || (firstMin.getDecimalPlaces() != newFirstMin.getDecimalPlaces()))
    {
        firstMin = newFirstMin;
        QSettings settings;
        settings.setValue("subtractionmodule/firstmin", firstMin.toString());
        settingsChanged = true;
    }

    if ((lastMax != newLastMax)
     || (lastMax.getDecimalPlaces() != newLastMax.getDecimalPlaces()))
    {
        lastMax = newLastMax;
        QSettings settings;
        settings.setValue("subtractionmodule/lastmax", lastMax.toString());
        settingsChanged = true;
    }

    if ((lastMin != newLastMin)
     || (lastMin.getDecimalPlaces() != newLastMin.getDecimalPlaces()))
    {
        lastMin = newLastMin;
        QSettings settings;
        settings.setValue("subtractionmodule/lastmin", lastMin.toString());
        settingsChanged = true;
    }

    // RESULTS
    if (this->largestNumberFirst != newLargestNumberFirst)
    {
        this->largestNumberFirst = newLargestNumberFirst;
        QSettings settings;
        settings.setValue("subtractionmodule/largestNumberFirst", largestNumberFirst);
        settingsChanged = true;
    }

    if (settingsChanged)
    {
        mainWindow->newQuestion();
    }
}

bool SubtractionModule::applyConfig()
{
    if (configFrame != 0)
    {
        return configFrame->applyConfig();
    }

    return false;
}
