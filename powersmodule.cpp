#include "powersmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <QtGui>
#include <QString>
#include "powersconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "mathdisplayform.h"
#include "random.h"

PowersModule::PowersModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    // Init sane defaults
    firstNumber = 0;
    power = 0;
    answer = 0;

    genFirst = 0;
    genPower = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("powersmodule");
    min = settings.value("min", 2).toULongLong();
    max = settings.value("max", 10).toULongLong();
    powerMin = settings.value("powermin", 2).toULongLong();
    powerMax = settings.value("powermax", 2).toULongLong();
    decimalPlaces = settings.value("decimalplaces", 0).toUInt();
    settings.endGroup();

    // Make config frame
    configFrame = new PowersConfigFrame();
    configFrame->setModule(this);
    configFrame->setMinimum(min);
    configFrame->setMaximum(max);
    configFrame->setPowerMinimum(powerMin);
    configFrame->setPowerMaximum(powerMax);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());

    firstRangeUpdated();
    powerRangeUpdated();
}

PowersModule::~PowersModule()
{
    assert(configFrame != 0);
    assert(displayFrame != 0);

    delete genFirst;
    genFirst = 0;

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
    assert(genFirst != 0);
    assert(genPower != 0);

    firstNumber = (*genFirst)();
    power = (*genPower)();
    answer = static_cast<qint64>(pow(firstNumber, power));

    QString q = QString("<math><msup><mi>%1</mi><mn>%2</mn></msup></math>\n")
                .arg(decimalize(firstNumber, decimalPlaces))
                .arg(decimalize((quint64)power, decimalPlaces));

    return q;
}

bool PowersModule::isCorrect(QString& answerGiven)
{
    //! \todo Handle decimal places
    quint64 answerNum = answerGiven.toULongLong();
    if (answerNum == this->answer)
    {
        return true;
    } else {
        return false;
    }
}

QString PowersModule::getAnswerString()
{
    return QString("%1^%2 = %3")
            .arg(decimalize(firstNumber, decimalPlaces))
            .arg(decimalize(power, decimalPlaces))
            .arg(decimalize(answer, decimalPlaces*2));
}

/*! Range of first number updated, so make a new PRG for it.
 * \todo Perhaps this should be generalized so the lastRangeUpdated function
 * doesn't just do the same with different data.
 */
void PowersModule::firstRangeUpdated()
{
    // Get rid of previous generators
    delete genFirst;
    genFirst = 0;

    // Make new generator
    quint64 minGen = min * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 maxGen = max * static_cast<quint64>(pow(10, decimalPlaces));

    genFirst = new RandomInt<quint64>(minGen, maxGen);
}

/*! Range of last number updated, so make a new PRG for it.
 */
void PowersModule::powerRangeUpdated()
{
    // Get rid of previous generator
    delete genPower;
    genPower = 0;

    // Make new generator
    quint64 minGen = powerMin * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 maxGen = powerMax * static_cast<quint64>(pow(10, decimalPlaces));

    genPower = new RandomInt<quint64>(minGen, maxGen);
}

void PowersModule::setMaximum(quint64 newMax)
{
    if (this->max != newMax) {
        this->max = newMax;
        QSettings settings;
        settings.setValue("powersmodule/max", max);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void PowersModule::setMinimum(quint64 newMin)
{
    if (this->min != newMin) {
        this->min = newMin;
        QSettings settings;
        settings.setValue("powersmodule/min", min);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void PowersModule::setPowerMaximum(quint32 newMax)
{
    if (this->powerMax != newMax) {
        this->powerMax = newMax;
        QSettings settings;
        settings.setValue("powersmodule/powermax", powerMax);

        powerRangeUpdated();
        mainWindow->newQuestion();
    }
}

void PowersModule::setPowerMinimum(quint32 newMin)
{
    if (this->powerMin != newMin) {
        this->powerMin = newMin;
        QSettings settings;
        settings.setValue("powersmodule/powermin", powerMin);

        powerRangeUpdated();
        mainWindow->newQuestion();
    }
}

void PowersModule::setDecimalPlaces(quint32 newDecimals)
{
    if (decimalPlaces != newDecimals) {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("powersmodule/decimalplaces", decimalPlaces);

        firstRangeUpdated();
        powerRangeUpdated();
        mainWindow->newQuestion();
    }
}
