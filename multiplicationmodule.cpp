#include "multiplicationmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <QtGui>
#include <string>
#include <QString>
#include "multiplicationconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "questiondisplayform.h"

MultiplicationModule::MultiplicationModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    this->mainWindow = mw;

    // Init sane defaults
    this->firstNumber = 0;
    this->lastNumber = 0;
    this->answer = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("multiplicationmodule");
    firstMin = settings.value("firstmin", 2).toULongLong();
    firstMax = settings.value("firstmax", 10).toULongLong();
    lastMin = settings.value("lastmin", 2).toULongLong();
    lastMax = settings.value("lastmax", 10).toULongLong();
    largestNumberFirst = settings.value("largestNumberFirst", false).toBool();
    //decimalPlaces = settings.value("decimalplaces", 0).toUInt();
    settings.endGroup();

    // Make config frame
    configFrame = new MultiplicationConfigFrame();
    configFrame->setModule(this);
    configFrame->setFirstMinimum(this->firstMin);
    configFrame->setFirstMaximum(this->firstMax);
    configFrame->setLastMinimum(this->lastMin);
    configFrame->setLastMaximum(this->lastMax);
    configFrame->setLargestNumberFirst(this->largestNumberFirst);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());

    // Seed random numbers
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));
}

MultiplicationModule::~MultiplicationModule()
{
    this->mainWindow->layout()->removeWidget(configFrame);
    configFrame->close();
    delete configFrame;
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

    this->firstNumber = (qrand() % (this->firstMax - this->firstMin + 1)) + this->firstMin;
    this->lastNumber = (qrand() % (this->lastMax - this->lastMin + 1)) + this->lastMin;
    if (this->largestNumberFirst && (this->lastNumber > this->firstNumber))
    {
        int tmp = this->firstNumber;
        this->firstNumber = this->lastNumber;
        this->lastNumber = tmp;
    }
    this->answer = this->firstNumber * this->lastNumber;

    QString q;
    q = QString::number(this->firstNumber) + "\nx " + QString::number(this->lastNumber);

    return q;
}

bool MultiplicationModule::isCorrect(QString& answerGiven)
{
    unsigned long answerNum = answerGiven.toULong();
    if (answerNum == this->answer)
    {
        return true;
    } else {
        return false;
    }
}

QString MultiplicationModule::getAnswerString()
{
    QString ans;
    ans = QString::number(this->firstNumber) + " x "
          + QString::number(this->lastNumber) + " = "
          + QString::number(this->answer);
    return ans;
}

void MultiplicationModule::setFirstMaximum(int newMax)
{
    if (this->firstMax != newMax) {
        this->firstMax = newMax;
        QSettings settings;
        settings.setValue("multiplicationmodule/firstmax", firstMax);

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setFirstMinimum(int newMin)
{
    if (this->firstMin != newMin) {
        this->firstMin = newMin;
        QSettings settings;
        settings.setValue("multiplicationmodule/firstmin", firstMin);

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setLastMaximum(int newMax)
{
    if (this->lastMax != newMax) {
        this->lastMax = newMax;
        QSettings settings;
        settings.setValue("multiplicationmodule/lastmax", lastMax);

        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setLastMinimum(int newMin)
{
    if (this->lastMin != newMin) {
        this->lastMin = newMin;
        QSettings settings;
        settings.setValue("multiplicationmodule/lastmin", lastMin);

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
