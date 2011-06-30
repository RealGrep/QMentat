#include "additionmodule.h"

#include <QtGui>
#include <QString>

#include <cassert>

#include "additionconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "questiondisplayform.h"
#include "random.h"

/*! Constructor for AdditionModule.
 *
 * \param mw MainWindow for module.
 */
AdditionModule::AdditionModule(MainWindow *mw)
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
    settings.beginGroup("additionmodule");
    firstMin = settings.value("firstmin", 2).toULongLong();
    firstMax = settings.value("firstmax", 100).toULongLong();
    lastMin = settings.value("lastmin", 2).toULongLong();
    lastMax = settings.value("lastmax", 100).toULongLong();
    largestNumberFirst = settings.value("largestNumberFirst", false).toBool();
    decimalPlaces = settings.value("decimalplaces", 0).toUInt();
    settings.endGroup();

    // Make config frame
    configFrame = new AdditionConfigFrame();
    configFrame->setModule(this);
    /*! \todo Decimal places must be set before setting the max/min values
    * since it affects the RNGs. Fix this dependency.
    */
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

AdditionModule::~AdditionModule()
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
    answer = firstNumber + lastNumber;

    QString q = QString("%1\n+ %2")
                .arg(decimalize(firstNumber, decimalPlaces))
                .arg(decimalize(lastNumber, decimalPlaces));

    return q;
}

/*! \param Reference to a QString containing the user's answer to the
 * current question.
 *  \returns true if the user answered correctly, false otherwise.
 *  \todo Perhaps all the fixed point stuff should go in it's own class/template.
 */
bool AdditionModule::isCorrect(QString& answerGiven)
{
    // Figure out how many decimal places we're "missing" and get them back
    // so the comparison works right
    int decimalPos = answerGiven.indexOf(QLocale::system().decimalPoint());
    quint64 answerNum = 0;
    if (decimalPos >= 0)
    {
        int missingDecimals = decimalPlaces - answerGiven.size() - 1
                              - decimalPos;
        answerNum = answerGiven.remove(QLocale::system().decimalPoint())
                    .remove(QLocale::system().groupSeparator()).toULongLong();
        if (missingDecimals > 0)
        {
            answerNum *= static_cast<quint64>(pow(10, missingDecimals));
        }
    } else {
        answerNum = answerGiven.toULongLong();
    }

    //qDebug() << "isCorrect: answerGiven = " << answerNum << "; answer = " << answer;
    if (answerNum == this->answer)
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
            .arg(decimalize(firstNumber, decimalPlaces))
            .arg(decimalize(lastNumber, decimalPlaces))
            .arg(decimalize(answer, decimalPlaces));
}

/*! Range of first number updated, so make a new PRG for it.
 * \todo Perhaps this should be generalized so the lastRangeUpdated function
 * doesn't just do the same with different data.
 */
void AdditionModule::firstRangeUpdated()
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
void AdditionModule::lastRangeUpdated()
{
    // Get rid of previous generator
    delete genLast;
    genLast = 0;

    // Make new generator
    quint64 min = lastMin * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 max = lastMax * static_cast<quint64>(pow(10, decimalPlaces));

    genLast = new RandomInt<quint64>(min, max);
}

void AdditionModule::setFirstMaximum(quint64 newMax)
{
    if (firstMax != newMax) {
        if (std::numeric_limits<quint64>::max() - lastMax <= newMax)
        {
            qDebug() << "OVERFLOW POSSIBLE!" << endl;

        } else {
            firstMax = newMax;
            QSettings settings;
            settings.setValue("additionmodule/firstmax", firstMax);

            firstRangeUpdated();
            mainWindow->newQuestion();
        }
    }
}

void AdditionModule::setFirstMinimum(quint64 newMin)
{
    if (firstMin != newMin) {
        firstMin = newMin;
        QSettings settings;
        settings.setValue("additionmodule/firstmin", firstMin);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

void AdditionModule::setLastMaximum(quint64 newMax)
{
    if (lastMax != newMax) {
        if (std::numeric_limits<quint64>::max() - newMax <= firstMax)
        {
            qDebug() << "OVERFLOW POSSIBLE!" << endl;

        } else {
            lastMax = newMax;
            QSettings settings;
            settings.setValue("additionmodule/lastmax", lastMax);

            lastRangeUpdated();
            mainWindow->newQuestion();
        }
    }
}

void AdditionModule::setLastMinimum(quint64 newMin)
{
    if (lastMin != newMin) {
        lastMin = newMin;
        QSettings settings;
        settings.setValue("additionmodule/lastmin", lastMin);

        lastRangeUpdated();
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

void AdditionModule::setDecimalPlaces(quint32 newDecimals)
{
    if (decimalPlaces != newDecimals) {
        decimalPlaces = newDecimals;

        QSettings settings;
        settings.setValue("additionmodule/decimalplaces", decimalPlaces);

        firstRangeUpdated();
        lastRangeUpdated();

        mainWindow->newQuestion();
    }
}
