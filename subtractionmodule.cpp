#include "subtractionmodule.h"

#include <QtGui>
//#include <string>
#include <QString>

#include <cassert>

//#include <iostream>
#include "subtractionconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "questiondisplayform.h"
#include "random.h"

SubtractionModule::SubtractionModule(MainWindow *mw)
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
    settings.beginGroup("subtractionmodule");
    firstMin = settings.value("firstmin", 2).toULongLong();
    firstMax = settings.value("firstmax", 100).toULongLong();
    lastMin = settings.value("lastmin", 2).toULongLong();
    lastMax = settings.value("lastmax", 100).toULongLong();
    largestNumberFirst = settings.value("largestNumberFirst", true).toBool();
    decimalPlaces = settings.value("decimalplaces", 0).toUInt();
    settings.endGroup();

    // Make config frame
    configFrame = new SubtractionConfigFrame();
    configFrame->setModule(this);
    configFrame->setDecimalPlaces(decimalPlaces);
    configFrame->setFirstMinimum(firstMin);
    configFrame->setFirstMaximum(firstMax);
    configFrame->setLastMinimum(lastMin);
    configFrame->setLastMaximum(lastMax);
    configFrame->setLargestNumberFirst(largestNumberFirst);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());

    firstRangeUpdated();
    lastRangeUpdated();
}

SubtractionModule::~SubtractionModule()
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
    answer = static_cast<qint64>(firstNumber) - static_cast<qint64>(lastNumber);

    QString q = QString("%1\n- %2")
                .arg(decimalize(firstNumber, decimalPlaces))
                .arg(decimalize(lastNumber, decimalPlaces));

    return q;
}

bool SubtractionModule::isCorrect(QString& answerGiven)
{
    // Figure out how many decimal places we're "missing" and get them back
    // so the comparison works right
    int decimalPos = answerGiven.indexOf(QLocale::system().decimalPoint());
    qint64 answerNum = 0;
    if (decimalPos >= 0)
    {
        int missingDecimals = decimalPlaces - answerGiven.size() - 1
                              - decimalPos;
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

QString SubtractionModule::getAnswerString()
{
    return QString("%1 - %2 = %3")
            .arg(decimalize(firstNumber, decimalPlaces))
            .arg(decimalize(lastNumber, decimalPlaces))
            .arg(decimalize(answer, decimalPlaces));
}

/*! Range of first number updated, so make a new PRG for it.
 * \todo Perhaps this should be generalized so the lastRangeUpdated function
 * doesn't just do the same with different data.
 */
void SubtractionModule::firstRangeUpdated()
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
void SubtractionModule::lastRangeUpdated()
{
    // Get rid of previous generator
    delete genLast;
    genLast = 0;

    // Make new generator
    quint64 min = lastMin * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 max = lastMax * static_cast<quint64>(pow(10, decimalPlaces));

    genLast = new RandomInt<quint64>(min, max);
}

void SubtractionModule::setFirstMaximum(quint64 newMax)
{
    if (firstMax != newMax) {
        firstMax = newMax;
        QSettings settings;
        settings.setValue("subtractionmodule/firstmax", firstMax);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void SubtractionModule::setFirstMinimum(quint64 newMin)
{
    if (firstMin != newMin) {
        firstMin = newMin;
        QSettings settings;
        settings.setValue("subtractionmodule/firstmin", firstMin);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void SubtractionModule::setLastMaximum(quint64 newMax)
{
    if (lastMax != newMax) {
        lastMax = newMax;
        QSettings settings;
        settings.setValue("subtractionmodule/lastmax", lastMax);

        lastRangeUpdated();
        mainWindow->newQuestion();
    }
}

void SubtractionModule::setLastMinimum(quint64 newMin)
{
    if (lastMin != newMin) {
        lastMin = newMin;
        QSettings settings;
        settings.setValue("subtractionmodule/lastmin", lastMin);

        lastRangeUpdated();
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

        firstRangeUpdated();
        lastRangeUpdated();
        mainWindow->newQuestion();
    }
}

void SubtractionModule::setDecimalPlaces(quint32 newDecimals)
{
    if (decimalPlaces != newDecimals) {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("subtractionmodule/decimalplaces", decimalPlaces);

        firstRangeUpdated();
        lastRangeUpdated();

        mainWindow->newQuestion();
    }
}
