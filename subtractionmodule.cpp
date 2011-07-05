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
    //std::cout << "first = " << firstNumber.toString().toStdString() << std::endl;
    //std::cout << "last = " << lastNumber.toString().toStdString() << std::endl;

    // Build question string
    QString q = QString("%1\n- %2")
                .arg(firstNumber.toString())
                .arg(lastNumber.toString());

    return q;
}

bool SubtractionModule::isCorrect(QString& answerGiven)
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

void SubtractionModule::setFirstMaximum(BigFixedPoint newMax)
{
    if ((firstMax != newMax)
        || (firstMax.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        firstMax = newMax;
        QSettings settings;
        settings.setValue("subtractionmodule/firstmax", firstMax.toString());

        mainWindow->newQuestion();
    }
}

void SubtractionModule::setFirstMinimum(BigFixedPoint newMin)
{
    if ((firstMin != newMin)
        || (firstMin.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        firstMin = newMin;
        QSettings settings;
        settings.setValue("subtractionmodule/firstmin", firstMin.toString());

        mainWindow->newQuestion();
    }
}

void SubtractionModule::setLastMaximum(BigFixedPoint newMax)
{
    if ((lastMax != newMax)
        || (lastMax.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        lastMax = newMax;
        QSettings settings;
        settings.setValue("subtractionmodule/lastmax", lastMax.toString());

        mainWindow->newQuestion();
    }
}

void SubtractionModule::setLastMinimum(BigFixedPoint newMin)
{
    if ((lastMin != newMin)
        || (lastMin.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        lastMin = newMin;
        QSettings settings;
        settings.setValue("subtractionmodule/lastmin", lastMin.toString());

        mainWindow->newQuestion();
    }
}

void SubtractionModule::setLargestNumberFirst(bool b)
{
    if (largestNumberFirst != b)
    {
        largestNumberFirst = b;
        QSettings settings;
        settings.setValue("subtractionmodule/largestNumberFirst",
                          largestNumberFirst);

        mainWindow->newQuestion();
    }
}
