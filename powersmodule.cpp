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
#include "bigfixedpoint.h"

PowersModule::PowersModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    genPower = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("powersmodule");
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

    // Make display frame
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());

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

    //std::cout << "first = " << firstNumber.toString().toStdString() << std::endl;

    // Build question string
    QString q = QString("<math><msup><mi>%1</mi><mn>%2</mn></msup></math>\n")
                .arg(firstNumber.toString())
                .arg(power);

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

void PowersModule::setMaximum(BigFixedPoint newMax)
{
    if ((max != newMax)
        || (max.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        max = newMax;
        QSettings settings;
        settings.setValue("powersmodule/max", max.toString());

        mainWindow->newQuestion();
    }
}

void PowersModule::setMinimum(BigFixedPoint newMin)
{
    if ((min != newMin)
        || (min.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        min = newMin;
        QSettings settings;
        settings.setValue("powersmodule/min", min.toString());

        mainWindow->newQuestion();
    }
}

void PowersModule::setPowerMaximum(int newMax)
{
    if (powerMax != newMax)
    {
        powerMax = newMax;
        QSettings settings;
        settings.setValue("powersmodule/powermax", powerMax);

        powerRangeUpdated();
        mainWindow->newQuestion();
    }
}

void PowersModule::setPowerMinimum(int newMin)
{
    if (powerMin != newMin)
    {
        powerMin = newMin;
        QSettings settings;
        settings.setValue("powersmodule/powermin", powerMin);

        powerRangeUpdated();
        mainWindow->newQuestion();
    }
}

void PowersModule::setDecimalPlaces(int newDecimals)
{
    if (decimalPlaces != newDecimals)
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("powersmodule/decimalplaces", decimalPlaces);

        mainWindow->newQuestion();
    }
}
