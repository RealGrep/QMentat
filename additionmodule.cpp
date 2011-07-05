#include "additionmodule.h"

#include <QtGui>
#include <QString>

#include <cassert>

#include "additionconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "questiondisplayform.h"
#include "bigfixedpoint.h"

/*! Constructor for AdditionModule.
 *
 * \param mw MainWindow for module.
 */
AdditionModule::AdditionModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    // Read config
    QSettings settings;
    settings.beginGroup("additionmodule");
    firstMin = BigFixedPoint(settings.value("firstmin", 2).toString());
    firstMax = BigFixedPoint(settings.value("firstmax", 100).toString());
    lastMin = BigFixedPoint(settings.value("lastmin", 2).toString());
    lastMax = BigFixedPoint(settings.value("lastmax", 100).toString());
    largestNumberFirst = settings.value("largestNumberFirst", false).toBool();
    settings.endGroup();

    // Make config frame
    configFrame = new AdditionConfigFrame();
    configFrame->setModule(this);
    configFrame->setFirstMinimum(firstMin.toString());
    configFrame->setFirstMaximum(firstMax.toString());
    configFrame->setLastMinimum(lastMin.toString());
    configFrame->setLastMaximum(lastMax.toString());
    configFrame->setLargestNumberFirst(largestNumberFirst);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());
}

AdditionModule::~AdditionModule()
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

/*! \returns Pointer to QFrame containing configuration form for this module.
 */
QFrame* AdditionModule::getConfigFrame()
{
    assert(configFrame != 0);
    return configFrame;
}

/*! \returns Pointer to QuestionDisplay form which is used to display questions
 * for this module.
 */
QuestionDisplay* AdditionModule::getDisplayFrame()
{
    assert(displayFrame != 0);
    return displayFrame;
}

/*! \returns A QString containing the question to be displayed to the user.
 * Must be in appropriate format for our DisplayFrame.
 */
QString AdditionModule::question()
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
    answer = firstNumber + lastNumber;
    //std::cout << "first = " << firstNumber.toString().toStdString() << std::endl;
    //std::cout << "last = " << lastNumber.toString().toStdString() << std::endl;

    // Build question string
    QString q = QString("%1\n+ %2")
                .arg(firstNumber.toString())
                .arg(lastNumber.toString());

    return q;
}

/*! \param Reference to a QString containing the user's answer to the
 * current question.
 *  \returns true if the user answered correctly, false otherwise.
 *  \todo Perhaps all the fixed point stuff should go in it's own class/template.
 */
bool AdditionModule::isCorrect(QString& answerGiven)
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

/*! \returns A QString containing the user viewable textual answer to the
 * current question.
 */
QString AdditionModule::getAnswerString()
{
    return QString("%1 + %2 = %3")
            .arg(firstNumber.toString())
            .arg(lastNumber.toString())
            .arg(answer.toString());
}

void AdditionModule::setFirstMaximum(BigFixedPoint newMax)
{
    if ((firstMax != newMax)
        || (firstMax.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        firstMax = newMax;
        QSettings settings;
        settings.setValue("additionmodule/firstmax", firstMax.toString());

        mainWindow->newQuestion();
    }
}

void AdditionModule::setFirstMinimum(BigFixedPoint newMin)
{
    if ((firstMin != newMin)
        || (firstMin.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        firstMin = newMin;
        QSettings settings;
        settings.setValue("additionmodule/firstmin", firstMin.toString());

        mainWindow->newQuestion();
    }
}

void AdditionModule::setLastMaximum(BigFixedPoint newMax)
{
    if ((lastMax != newMax)
        || (lastMax.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        lastMax = newMax;
        QSettings settings;
        settings.setValue("additionmodule/lastmax", lastMax.toString());

        mainWindow->newQuestion();
    }
}

void AdditionModule::setLastMinimum(BigFixedPoint newMin)
{
    if ((lastMin != newMin)
        || (lastMin.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        lastMin = newMin;
        QSettings settings;
        settings.setValue("additionmodule/lastmin", lastMin.toString());

        mainWindow->newQuestion();
    }
}

void AdditionModule::setLargestNumberFirst(bool b)
{
    if (largestNumberFirst != b)
    {
        largestNumberFirst = b;
        QSettings settings;
        settings.setValue("additionmodule/largestNumberFirst",
                          largestNumberFirst);
        mainWindow->newQuestion();
    }
}
