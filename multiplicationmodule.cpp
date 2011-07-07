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
    //range_type result_increment = uniform_int<range_type>(0, range/mult)(eng);
    //        if((std::numeric_limits<range_type>::max)() / mult < result_increment) {
    //          // The multiplcation would overflow.  Reject immediately.
    //          continue;
    //        }

    firstNumber = BigFixedPoint::random(firstMin, firstMax);
    lastNumber = BigFixedPoint::random(lastMin, lastMax);

    // Scale to largest number of decimals
    /*
    int maxDecimals = std::max(firstNumber.getDecimalPlaces(), lastNumber.getDecimalPlaces());
    if (firstNumber.getDecimalPlaces() < maxDecimals)
    {
        firstNumber.scale(maxDecimals);
    }
    if (lastNumber.getDecimalPlaces() < maxDecimals)
    {
        lastNumber.scale(maxDecimals);
    }
    */

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
    assert(decimalPlaces <= answer.getDecimalPlaces());
    answer.scale(decimalPlaces);
    //std::cout << "first = " << firstNumber.toString().toStdString() << std::endl;
    //std::cout << "last = " << lastNumber.toString().toStdString() << std::endl;

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

void MultiplicationModule::setFirstMaximum(BigFixedPoint newMax)
{
    if ((firstMax != newMax)
        || (firstMax.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        firstMax = newMax;
        QSettings settings;
        settings.setValue("multiplicationmodule/firstmax", firstMax.toString());

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setFirstMinimum(BigFixedPoint newMin)
{
    if ((firstMin != newMin)
        || (firstMin.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        firstMin = newMin;
        QSettings settings;
        settings.setValue("multiplicationmodule/firstmin", firstMin.toString());

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setLastMaximum(BigFixedPoint newMax)
{
    if ((lastMax != newMax)
        || (lastMax.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        lastMax = newMax;
        QSettings settings;
        settings.setValue("multiplicationmodule/lastmax", lastMax.toString());

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setLastMinimum(BigFixedPoint newMin)
{
    if ((lastMin != newMin)
        || (lastMin.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        lastMin = newMin;
        QSettings settings;
        settings.setValue("multiplicationmodule/lastmin", lastMin.toString());

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setLargestNumberFirst(bool b)
{
    if (this->largestNumberFirst != b)
    {
        this->largestNumberFirst = b;
        QSettings settings;
        settings.setValue("multiplicationmodule/largestNumberFirst",
                          largestNumberFirst);

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setDecimalPlaces(int newDecimals)
{
    if ((decimalPlaces != newDecimals)
        && (newDecimals >= 0)
        && (newDecimals <= firstNumber.getDecimalPlaces()+lastNumber.getDecimalPlaces()))
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("multiplicationmodule/decimalplaces", decimalPlaces);

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setRoundingMode(bool rnd)
{
    if (roundingMode != rnd)
    {
        roundingMode = rnd;
        BigFixedPoint::setRounding(roundingMode);
        QSettings settings;
        settings.setValue("multiplicationmodule/roundingmode", roundingMode);

        mainWindow->newQuestion();
    }
}
