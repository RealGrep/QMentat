#include "divisionmodule.h"

#include "mainwindow.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <QtGui>
#include <string>
#include <QString>
#include "divisionconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "mathdisplayform.h"
#include "random.h"
#include "bigfixedpoint.h"

/*! \class DivisionModule
 *  \brief Handles logic of questions and answers for division practice.
 *  \author Mike Dusseault <mike.dusseault@gmail.com>
 */
DivisionModule::DivisionModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    mainWindow = mw;

    genFirst = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("divisionmodule");
    roundingMode = settings.value("roundingmode", false).toBool();
    BigFixedPoint::setRounding(roundingMode == DivisionConfigFrame::ROUNDING_ROUND);
    integersOnly = settings.value("integersonly", false).toBool();

    if (integersOnly)
    {
        QString str = settings.value("firstmin", 2).toString();
        firstMinIR = str.remove(QLocale::system().groupSeparator()).toLongLong();
        str = settings.value("firstmax", 100).toString();
        firstMaxIR = str.remove(QLocale::system().groupSeparator()).toLongLong();
        str = settings.value("lastmin", 2).toString();
        lastMinIR = str.remove(QLocale::system().groupSeparator()).toLongLong();
        str = settings.value("lastmax", 100).toString();
        lastMaxIR = str.remove(QLocale::system().groupSeparator()).toLongLong();
    } else {
        firstMin = BigFixedPoint(settings.value("firstmin", 2).toString());
        firstMax = BigFixedPoint(settings.value("firstmax", 100).toString());
        lastMin = BigFixedPoint(settings.value("lastmin", 2).toString());
        lastMax = BigFixedPoint(settings.value("lastmax", 100).toString());
    }
    largestNumberFirst = settings.value("largestNumberFirst", false).toBool();
    decimalPlaces = settings.value("decimalplaces", 0).toInt();
    settings.endGroup();

    // Make config frame
    configFrame = new DivisionConfigFrame();
    configFrame->setModule(this);
    if (integersOnly)
    {
        configFrame->setFirstMinimum(QString("%L1").arg(firstMinIR));
        configFrame->setFirstMaximum(QString("%L1").arg(firstMaxIR));
        configFrame->setLastMinimum(QString("%L1").arg(lastMinIR));
        configFrame->setLastMaximum(QString("%L1").arg(lastMaxIR));
    } else {
        configFrame->setFirstMinimum(firstMin.toString());
        configFrame->setFirstMaximum(firstMax.toString());
        configFrame->setLastMinimum(lastMin.toString());
        configFrame->setLastMaximum(lastMax.toString());
    }
    configFrame->setLargestNumberFirst(largestNumberFirst);
    configFrame->setDecimalPlaces(decimalPlaces);
    configFrame->setRoundingMode((roundingMode == true)
                                 ? DivisionConfigFrame::ROUNDING_ROUND
                                     : DivisionConfigFrame::ROUNDING_TRUNCATE);
    configFrame->setIntegersOnly(integersOnly);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());

    if (integersOnly)
    {
        firstRangeUpdated();
    }
}

DivisionModule::~DivisionModule()
{
    assert(configFrame != 0);
    assert(displayFrame != 0);

    delete genFirst;
    genFirst = 0;

    this->mainWindow->layout()->removeWidget(configFrame);
    configFrame->close();
    delete configFrame;
    configFrame = 0;

    delete displayFrame;
    displayFrame = 0;
}

/*! Return config frame.
 */
QFrame* DivisionModule::getConfigFrame()
{
    return configFrame;
}

/*! Return display frame.
 */
QuestionDisplay* DivisionModule::getDisplayFrame()
{
    return displayFrame;
}

/*! Tests to see if any divisors divide any of the possible dividends.
 *  \param firstMin Minimum of the dividend.
 *  \param firstMax Maximum of the dividend.
 *  \param lastMin Minimum of the divisor.
 *  \param lastMax Maximum of the divisor.
 *  \returns true if at least one of the possible divisors divides at
 *    least one of the possible dividends, false otherwise.
 */
bool DivisionModule::isRangeOk(qint64 firstMin, qint64 firstMax,
                               qint64 lastMin, qint64 lastMax) const
{
    bool isOk = false;

    // We ignore firstMax because we don't care if it's only divisible by itself
    for (qint64 i = firstMax-1; i >= firstMin; --i)
    {
        qint64 last = std::min(static_cast<qint64>(sqrt(i)), lastMax) + 1;
        for (qint64 j = lastMin; j < last; ++j)
        {
            if (((i % j) == 0) && (i != j))
            {
                isOk = true;
                break;
            }
        }
        if (isOk)
            break;
    }

    return isOk;
}

/*! Finds divisors in a certain range for a number.
 * \param num Number for which we need divisors.
 * \param min Minimum divisor to consider.
 * \param max Maximum divisor to consider.
 * \returns Vector of divisors between min and max of num.
 */
std::vector<qint64> *DivisionModule::getDivisors(qint64 num,
                                                 qint64 min,
                                                 qint64 max) const
{
    std::vector<qint64> *divisors = new std::vector<qint64>();
    // No use going past sqrt(num)
    qint64 last = std::min(static_cast<qint64>(sqrt(num)), max) + 1;

    const int partitions = 100000000;
    QProgressDialog progress("Finding divisors, please wait...", "Cancel",
                             0, partitions, configFrame);
    progress.setWindowModality(Qt::WindowModal);
    int prog = 0;
    for (qint64 i = min; i <= last; ++i)
    {
        int newProg = (int)round(((double)(i - min) / (double)last) * (double)partitions);

        if (newProg > prog)
        {
            prog = newProg;
        }
        progress.setValue(prog);

        if (progress.wasCanceled())
        {
            /*
            if (divisors->empty())
            {
                delete divisors;
                divisors = 0;
                return 0;
            }
            */
            break;
        }

        if ((num % i) == 0)
        {
            divisors->push_back(i);
            if ((num / i) > last)
            {
                divisors->push_back(num/i);
            }
        }
    }

    progress.setValue(partitions);

    return divisors;
}

/*! \returns QString containing new question in MathML format.
 */
QString DivisionModule::question()
{
    if (integersOnly)
    {
        assert(genFirst != 0);

        QApplication::setOverrideCursor(Qt::WaitCursor);

        // Ensure we don't get a division by 0
        do {
            // Generate the numbers
            firstNumberIR = (*genFirst)();

            std::vector<qint64> *divisors = getDivisors(firstNumberIR,
                                                        lastMinIR, lastMaxIR);
            assert(divisors != 0);
            /*
            std::cout << "Divisors for " << firstNumberIR << ": ";
            for (auto it = divisors->begin(); it != divisors->end(); ++it)
            {
                std::cout << (*it) << " ";
            }
            std::cout << std::endl;
            */
            if (divisors->empty()) {
                // No divisors, try again
                lastNumberIR = 0;
                delete divisors;
                continue;
            } else {
                int random = qrand() % divisors->size();
                lastNumberIR = (qint64)(*divisors)[random];
                delete divisors;
            }

            answerIR = static_cast<quint64>(firstNumberIR)
                       / static_cast<quint64>(lastNumberIR);

        } while (lastNumberIR == 0);

        QApplication::restoreOverrideCursor();

        QString q = QString("<math><mfrac><mi>%L1</mi><mn>%L2</mn></mfrac></math>\n")
                    .arg(firstNumberIR)
                    .arg(lastNumberIR);

        return q;
    } else {
        firstNumber = BigFixedPoint::random(firstMin, firstMax);
        lastNumber = BigFixedPoint::random(lastMin, lastMax);

        // Swap largest number first, if necessary and set to do so
        if (largestNumberFirst && (lastNumber > firstNumber))
        {
            BigFixedPoint tmp = firstNumber;
            firstNumber = lastNumber;
            lastNumber = tmp;
        }

        // Calculate answer
        BigFixedPoint firstNumberScaled = firstNumber;
        firstNumberScaled.scale(firstNumber.getDecimalPlaces()+lastNumber.getDecimalPlaces()+decimalPlaces+1);
        answer = firstNumberScaled / lastNumber;
        assert(decimalPlaces >= 0);
        answer.scale(decimalPlaces);

        int decimals = 0;
        decimals = std::max(firstNumber.getDecimalPlaces(), lastNumber.getDecimalPlaces());

        BigFixedPoint firstDisplay(firstNumber);
        firstDisplay.scale(decimals);
        BigFixedPoint lastDisplay(lastNumber);
        lastDisplay.scale(decimals);

        // Build question string
        QString q = QString("<math><mfrac><mi>%1</mi><mn>%2</mn></mfrac></math>\n")
                    .arg(firstDisplay.toString())
                    .arg(lastDisplay.toString());

        return q;
    }
}

/*! Tells if the supplied answer is correct.
 * \returns true if correct, false otherwise.
 */
bool DivisionModule::isCorrect(QString& answerGiven)
{
    if (integersOnly)
    {
        qint64 answerNum = answerGiven.toLongLong();
        //qDebug() << "isCorrect(qint64): answerGiven = " << answerNum << "; answerIR = " << answerIR;
        if (answerNum == answerIR)
        {
            return true;
        } else {
            return false;
        }
    } else {
        BigFixedPoint answerNum(answerGiven);
        answerNum.scale(answer.getDecimalPlaces());

        //qDebug() << "isCorrect(decimals): answerGiven = " << answerNum.toString() << "; answer = " << answer.toString();
        if (answerNum == answer)
        {
            return true;
        } else {
            return false;
        }
    }
}

/*! Produces answer string for current question.
 * \returns QString containing full answer for display.
 */
QString DivisionModule::getAnswerString()
{
    if (integersOnly)
    {
        return QString("%L1 / %L2 = %L3")
                .arg(firstNumberIR)
                .arg(lastNumberIR)
                .arg(answerIR);
    } else {
        return QString("%1 / %2 = %3")
                .arg(firstNumber.toString())
                .arg(lastNumber.toString())
                .arg(answer.toString());
    }
}

void DivisionModule::setSettings(BigFixedPoint newFirstMin,
                                 BigFixedPoint newFirstMax,
                                 BigFixedPoint newLastMin,
                                 BigFixedPoint newLastMax,
                                 bool newLargestNumberFirst, bool newIntsOnly,
                                 int newDecimals, bool newRoundingMode)
{
    bool settingsChanged = false;

    // RANGE
    if ((firstMax != newFirstMax)
     || (firstMax.getDecimalPlaces() != newFirstMax.getDecimalPlaces()))
    {
        firstMax = newFirstMax;
        QSettings settings;
        settings.setValue("divisionmodule/firstmax", firstMax.toString());
        settingsChanged = true;
    }

    if ((firstMin != newFirstMin)
     || (firstMin.getDecimalPlaces() != newFirstMin.getDecimalPlaces()))
    {
        firstMin = newFirstMin;
        QSettings settings;
        settings.setValue("divisionmodule/firstmin", firstMin.toString());
        settingsChanged = true;
    }

    if ((lastMax != newLastMax)
     || (lastMax.getDecimalPlaces() != newLastMax.getDecimalPlaces()))
    {
        lastMax = newLastMax;
        QSettings settings;
        settings.setValue("divisionmodule/lastmax", lastMax.toString());
        settingsChanged = true;
    }

    if ((lastMin != newLastMin)
     || (lastMin.getDecimalPlaces() != newLastMin.getDecimalPlaces()))
    {
        lastMin = newLastMin;
        QSettings settings;
        settings.setValue("divisionmodule/lastmin", lastMin.toString());
        settingsChanged = true;
    }

    // RESULTS
    if (this->largestNumberFirst != newLargestNumberFirst)
    {
        this->largestNumberFirst = newLargestNumberFirst;
        QSettings settings;
        settings.setValue("divisionmodule/largestNumberFirst", largestNumberFirst);
        settingsChanged = true;
    }

    if (decimalPlaces != newDecimals)
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("divisionmodule/decimalplaces", decimalPlaces);
        settingsChanged = true;
    }

    if (roundingMode != newRoundingMode)
    {
        roundingMode = newRoundingMode;
        BigFixedPoint::setRounding(roundingMode);
        QSettings settings;
        settings.setValue("divisionmodule/roundingmode", roundingMode);
        settingsChanged = true;
    }

    if (integersOnly != newIntsOnly)
    {
        integersOnly = newIntsOnly;
        QSettings settings;
        settings.setValue("divisionmodule/integersonly", integersOnly);
        settingsChanged = true;
    }

    if (settingsChanged)
    {
        mainWindow->newQuestion();
    }
}

// For integer results only mode
void DivisionModule::setSettings(qint64 newFirstMin, qint64 newFirstMax,
                                 qint64 newLastMin, qint64 newLastMax,
                                 bool newlargestNumberFirst, bool newIntsOnly,
                                 int newDecimals, bool newRoundingMode)
{
    bool settingsChanged = false;

    if (firstMaxIR != newFirstMax)
    {
        firstMaxIR = newFirstMax;
        QSettings settings;
        settings.setValue("divisionmodule/firstmax", QString("%1").arg(firstMaxIR));
        settingsChanged = true;
    }

    if (firstMinIR != newFirstMin)
    {
        firstMinIR = newFirstMin;
        QSettings settings;
        settings.setValue("divisionmodule/firstmin", QString("%1").arg(firstMinIR));
        settingsChanged = true;
    }

    if (lastMaxIR != newLastMax)
    {
        lastMaxIR = newLastMax;
        QSettings settings;
        settings.setValue("divisionmodule/lastmax", QString("%1").arg(lastMaxIR));
        settingsChanged = true;
    }

    if (lastMinIR != newLastMin)
    {
        lastMinIR = newLastMin;
        QSettings settings;
        settings.setValue("divisionmodule/lastmin", QString("%1").arg(lastMinIR));
        settingsChanged = true;
    }

    // RESULTS
    if (this->largestNumberFirst != newlargestNumberFirst)
    {
        this->largestNumberFirst = newlargestNumberFirst;
        QSettings settings;
        settings.setValue("divisionmodule/largestNumberFirst", largestNumberFirst);
        settingsChanged = true;
    }

    if (decimalPlaces != newDecimals)
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("divisionmodule/decimalplaces", decimalPlaces);
        settingsChanged = true;
    }

    if (roundingMode != newRoundingMode)
    {
        roundingMode = newRoundingMode;
        BigFixedPoint::setRounding(roundingMode);
        QSettings settings;
        settings.setValue("divisionmodule/roundingmode", roundingMode);
        settingsChanged = true;
    }

    if (integersOnly != newIntsOnly)
    {
        integersOnly = newIntsOnly;
        QSettings settings;
        settings.setValue("divisionmodule/integersonly", integersOnly);
        settingsChanged = true;
    }

    if (settingsChanged)
    {
        firstRangeUpdated();
        mainWindow->newQuestion();
    }

}

void DivisionModule::firstRangeUpdated()
{
    // Get rid of previous generator
    delete genFirst;
    genFirst = 0;

    // Make new generator
    qint64 minGen = std::min(firstMinIR, firstMaxIR);
    qint64 maxGen = std::max(firstMaxIR, firstMinIR);

    genFirst = new RandomInt<qint64>(minGen, maxGen);
}

bool DivisionModule::applyConfig()
{
    if (configFrame != 0)
    {
        return configFrame->applyConfig();
    }

    return false;
}
