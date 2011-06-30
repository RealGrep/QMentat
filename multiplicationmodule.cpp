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

    // Init sane defaults
    firstNumber = 0;
    lastNumber = 0;
    answer = 0;

    genFirst = 0;
    genLast = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("multiplicationmodule");
    firstMin = settings.value("firstmin", 2).toULongLong();
    firstMax = settings.value("firstmax", 10).toULongLong();
    lastMin = settings.value("lastmin", 2).toULongLong();
    lastMax = settings.value("lastmax", 10).toULongLong();
    largestNumberFirst = settings.value("largestNumberFirst", false).toBool();
    decimalPlaces = settings.value("decimalplaces", 0).toUInt();
    settings.endGroup();

    // Make config frame
    configFrame = new MultiplicationConfigFrame();
    configFrame->setModule(this);
    configFrame->setFirstMinimum(firstMin);
    configFrame->setFirstMaximum(firstMax);
    configFrame->setLastMinimum(lastMin);
    configFrame->setLastMaximum(lastMax);
    configFrame->setLargestNumberFirst(largestNumberFirst);
    configFrame->setDecimalPlaces(decimalPlaces);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());

    firstRangeUpdated();
    lastRangeUpdated();
}

MultiplicationModule::~MultiplicationModule()
{
    assert(configFrame != 0);
    assert(displayFrame != 0);

    delete genFirst;
    genFirst = 0;

    delete genLast;
    genLast = 0;

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
    assert(genFirst != 0);
    assert(genLast != 0);

    firstNumber = (*genFirst)();
    lastNumber = (*genLast)();
    if (largestNumberFirst && (lastNumber > firstNumber))
    {
        int tmp = firstNumber;
        firstNumber = lastNumber;
        lastNumber = tmp;
    }
    answer = static_cast<qint64>(firstNumber) * static_cast<qint64>(lastNumber);

    QString q = QString("%1\nx %2")
                .arg(decimalize(firstNumber, decimalPlaces))
                .arg(decimalize(lastNumber, decimalPlaces));

    return q;
}

bool MultiplicationModule::isCorrect(QString& answerGiven)
{
    // Figure out how many decimal places we're "missing" and get them back
    // so the comparison works right
    int decimalPos = answerGiven.indexOf(QLocale::system().decimalPoint());
    quint64 answerNum = 0;
    if (decimalPos >= 0)
    {
        int missingDecimals = decimalPlaces*2 - (answerGiven.size() - 1
                              - decimalPos);
        answerNum = answerGiven.remove(QLocale::system().decimalPoint())
                    .remove(QLocale::system().groupSeparator()).toULongLong();
        if (missingDecimals > 0)
        {
            answerNum *= static_cast<qint64>(pow(10, missingDecimals));
        }
    } else {
        answerNum = answerGiven.toLongLong();
    }

    //qDebug() << "isCorrect: answerGiven = " << answerNum << "; answer = " << answer;
    if (answerNum == this->answer)
    {
        return true;
    } else {
        return false;
    }
}

QString MultiplicationModule::getAnswerString()
{
    return QString("%1 x %2 = %3")
            .arg(decimalize(firstNumber, decimalPlaces))
            .arg(decimalize(lastNumber, decimalPlaces))
            .arg(decimalize(answer, decimalPlaces*2));
}

/*! Range of first number updated, so make a new PRG for it.
 * \todo Perhaps this should be generalized so the lastRangeUpdated function
 * doesn't just do the same with different data.
 */
void MultiplicationModule::firstRangeUpdated()
{
    // Get rid of previous generators
    delete genFirst;
    genFirst = 0;

    // Make new generator
    quint64 min = firstMin * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 max = firstMax * static_cast<quint64>(pow(10, decimalPlaces));

    genFirst = new RandomInt<quint64>(min, max);
}

/*! Range of last number updated, so make a new PRG for it.
 */
void MultiplicationModule::lastRangeUpdated()
{
    // Get rid of previous generator
    delete genLast;
    genLast = 0;

    // Make new generator
    quint64 min = lastMin * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 max = lastMax * static_cast<quint64>(pow(10, decimalPlaces));

    genLast = new RandomInt<quint64>(min, max);
}

void MultiplicationModule::setFirstMaximum(quint64 newMax)
{
    if (this->firstMax != newMax) {
        this->firstMax = newMax;
        QSettings settings;
        settings.setValue("multiplicationmodule/firstmax", firstMax);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setFirstMinimum(quint64 newMin)
{
    if (this->firstMin != newMin) {
        this->firstMin = newMin;
        QSettings settings;
        settings.setValue("multiplicationmodule/firstmin", firstMin);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setLastMaximum(quint64 newMax)
{
    if (this->lastMax != newMax) {
        this->lastMax = newMax;
        QSettings settings;
        settings.setValue("multiplicationmodule/lastmax", lastMax);

        lastRangeUpdated();
        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setLastMinimum(quint64 newMin)
{
    if (this->lastMin != newMin) {
        this->lastMin = newMin;
        QSettings settings;
        settings.setValue("multiplicationmodule/lastmin", lastMin);

        lastRangeUpdated();
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

        firstRangeUpdated();
        lastRangeUpdated();
        mainWindow->newQuestion();
    }
}

void MultiplicationModule::setDecimalPlaces(quint32 newDecimals)
{
    if (decimalPlaces != newDecimals) {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("multiplicationmodule/decimalplaces", decimalPlaces);

        firstRangeUpdated();
        lastRangeUpdated();
        mainWindow->newQuestion();
    }
}
