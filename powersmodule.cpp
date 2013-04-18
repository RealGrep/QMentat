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
#include "powersmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <QtGui>
#include <QString>
#include "powersconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"

#ifdef USE_MATH_DISPLAY
#   include "mathdisplayform.h"
#else
#   include "questiondisplayform.h"
#endif

#include "random.h"
#include "bigfixedpoint.h"

PowersModule::PowersModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    // Init sane defaults
    power = 0;
    genPower = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("powersmodule");
    roundingMode = settings.value("roundingmode", false).toBool();
    BigFixedPoint::setRounding(roundingMode == 1);
    min = BigFixedPoint(settings.value("min", 2).toString());
    max = BigFixedPoint(settings.value("max", 10).toString());
    powerMin = settings.value("powermin", 2).toInt();
    powerMax = settings.value("powermax", 2).toInt();
    decimalPlaces = settings.value("decimalplaces", 0).toInt();
    settings.endGroup();

    // Make config frame
    configFrame = new PowersConfigFrame();
    configFrame->setModule(this);
    configFrame->setMinimum(min.toString());
    configFrame->setMaximum(max.toString());
    configFrame->setPowerMinimum(powerMin);
    configFrame->setPowerMaximum(powerMax);
    configFrame->setDecimalPlaces(decimalPlaces);
    configFrame->setRoundingMode((roundingMode == true) ? 1 : 0);

    // Make display frame
#ifdef USE_MATH_DISPLAY
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());
#else
    displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());
#endif

    powerRangeUpdated();
}

PowersModule::~PowersModule()
{
    assert(configFrame != 0);
    assert(displayFrame != 0);

    delete genPower;
    genPower = 0;

    this->mainWindow->layout()->removeWidget(configFrame);
    configFrame->close();
    delete configFrame;
    configFrame = 0;

    delete displayFrame;
    displayFrame = 0;
}

QFrame* PowersModule::getConfigFrame()
{
    return configFrame;
}

QuestionDisplay* PowersModule::getDisplayFrame()
{
    return displayFrame;
}

QString PowersModule::question()
{
    assert(genPower != 0);

    firstNumber = BigFixedPoint::random(min, max);
    power = (*genPower)();

    // Calculate answer
    answer = firstNumber.pow(power);
    answer.scale(decimalPlaces);

    // Build question string
#ifdef USE_MATH_DISPLAY
    QString q = QString("<math><msup><mi>%1</mi><mn>%2</mn></msup></math>\n")
                .arg(firstNumber.toString())
                .arg(power);
#else
    QString q = QString("%1\n^ %2").arg(firstNumber.toString()).arg(power);
#endif

    return q;
}

bool PowersModule::isCorrect(QString& answerGiven)
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

QString PowersModule::getAnswerString()
{
    return QString("%1^%2 = %3")
            .arg(firstNumber.toString())
            .arg(power)
            .arg(answer.toString());
}

/*! Range of last number updated, so make a new PRG for it.
 */
void PowersModule::powerRangeUpdated()
{
    // Get rid of previous generator
    delete genPower;
    genPower = 0;

    // Make new generator
    int minGen = powerMin;
    int maxGen = powerMax;

    genPower = new RandomInt<int>(minGen, maxGen);
}

void PowersModule::setSettings(BigFixedPoint newFirstMin,
                               BigFixedPoint newFirstMax,
                               int newPowerMin,
                               int newPowerMax,
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
        settings.setValue("powersmodule/max", max.toString());
        settingsChanged = true;
    }

    if ((min != newFirstMin)
     || (min.getDecimalPlaces() != newFirstMin.getDecimalPlaces()))
    {
        min = newFirstMin;
        QSettings settings;
        settings.setValue("powersmodule/min", min.toString());
        settingsChanged = true;
    }

    if (powerMax != newPowerMax)
    {
        powerMax = newPowerMax;
        QSettings settings;
        settings.setValue("powersmodule/powermax", powerMax);
        settingsChanged = true;
    }

    if (powerMin != newPowerMin)
    {
        powerMin = newPowerMin;
        QSettings settings;
        settings.setValue("powersmodule/powermin", powerMin);
        settingsChanged = true;
    }

    // RESULTS
    if (decimalPlaces != newDecimals)
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("powersmodule/decimalplaces", decimalPlaces);
        settingsChanged = true;
    }

    if (roundingMode != newRoundingMode)
    {
        roundingMode = newRoundingMode;
        BigFixedPoint::setRounding(roundingMode);
        QSettings settings;
        settings.setValue("powersmodule/roundingmode", roundingMode);
        settingsChanged = true;
    }

    if (settingsChanged)
    {
        powerRangeUpdated();
        mainWindow->newQuestion();
    }
}

bool PowersModule::applyConfig()
{
    if (configFrame != 0)
    {
        return configFrame->applyConfig();
    }

    return false;
}
