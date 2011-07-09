#include "divisionmodule.h"

#include "mainwindow.h"
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
    BigFixedPoint::setRounding(roundingMode == 1);
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
    configFrame->setRoundingMode((roundingMode == true) ? 1 : 0);
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
                               qint64 lastMin, qint64 lastMax)
{
    bool isOk = false;

    // We ignore firstMax because we don't care if it's only divisible by itself
    for (qint64 i = firstMax-1; i >= firstMin; --i)
    {
        for (qint64 j = lastMin; j < lastMax; ++j)
        {
            //std::cout << "[" << i << "/" << j << "]" << std::endl;
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
                                                 qint64 max)
{
    std::vector<qint64> *divisors = new std::vector<qint64>();
    //std::cout << num << " is divisible by ";
    // No use going past sqrt(num)
    //qint64 last = std::min(num/2, max) + 1;
    qint64 last = std::min(static_cast<qint64>(sqrt(num)), max) + 1;
    //std::cout << "[" << min << "->" << last << "] ";
    for (unsigned long i = min; i <= last; ++i)
    {
        if ((num % i) == 0)
        {
            //isOk = true;
            //std::cout << i << " ";

            divisors->push_back(i);
            if ((num / i) > last)
            {
                divisors->push_back(num/i);
            }
        }
    }
    //std::cout << std::endl;

    return divisors;
}
#if 0
std::vector<BigFixedPoint> *DivisionModule::getDivisors(BigFixedPoint& num,
                                                        BigFixedPoint& min,
                                                        BigFixedPoint& max)
{
    std::vector<BigFixedPoint> *divisors = new std::vector<BigFixedPoint>();
    //qDebug() << num.toString() << " is divisible by ";
    // No use going past num/2
    BigFixedPoint minScaled(min);
    minScaled.scale(num.getDecimalPlaces());
    BigFixedPoint maxScaled(max);
    maxScaled.scale(num.getDecimalPlaces());
    BigFixedPoint last = BigFixedPoint::min(num/2, maxScaled) + 1;
    //qDebug() << "[" << min.toString() << "->" << last.toString() << "] ";
    for (BigFixedPoint i = minScaled; i < last; i += 1)
    {
        if ((num % i) == 0)
        {
            //isOk = true;
            //qDebug() << i.toString() << " ";
            divisors->push_back(BigFixedPoint(i));
        }
    }
    //std::cout << std::endl;

    return divisors;
}
#endif

/*! \returns QString containing new question in MathML format.
 */
QString DivisionModule::question()
{
    if (integersOnly)
    {
        assert(genFirst != 0);

        //mainWindow->showMessage(QString("Finding divisors, please wait, as this can be slow for large number..."));
        //mainWindow->statusBar()->show();

        // Check range
        // TODO: Fix it so it can never happen
        bool rangeOk = isRangeOk(firstMinIR, firstMaxIR,
                                 lastMinIR, lastMaxIR);
        if (!rangeOk) return "<math>ERR</math>";

        // Ensure we don't get a division by 0
        do {
            // Generate the numbers
            firstNumberIR = (*genFirst)();

            std::vector<qint64> *divisors = getDivisors(firstNumberIR,
                                                        lastMinIR, lastMaxIR);
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
                //std::cout << "No divisors!" << std::endl;
                lastNumberIR = 0;
                delete divisors;
                continue;
            } else {
                //! \todo Seeeeed mee.....
                int random = qrand() % divisors->size();
                lastNumberIR = (qint64)(*divisors)[random];
                delete divisors;
            }

            answerIR = static_cast<quint64>(firstNumberIR)
                       / static_cast<quint64>(lastNumberIR);

        } while (lastNumberIR == 0);

        //mainWindow->statusBar()->showMessage(QString(""));
        QString q = QString("<math><mfrac><mi>%L1</mi><mn>%L2</mn></mfrac></math>\n")
                    .arg(firstNumberIR)
                    .arg(lastNumberIR);

        return q;
#if 0
        // Ensure we don't get a division by 0
        int i = 0;
        do {
            // Generate the numbers
            firstNumber = BigFixedPoint::random(firstMin, firstMax);
            //qDebug() << "firstNumber = " << firstNumber.toString();

            std::vector<BigFixedPoint> *divisors = getDivisors(firstNumber,
                                                               lastMin, lastMax);
            if (divisors->empty()) {
                // No divisors, try again
                //std::cout << "No divisors!" << std::endl;
                lastNumber = 0;
                delete divisors;
                continue;
            } else {
                //! \todo Use a proper PRNG
                int random = qrand() % divisors->size();
                lastNumber = (*divisors)[random];
                delete divisors;
            }

            answer = firstNumber / lastNumber;
            answer.scale(0);
            ++i;
        } while ((i < 1000) && (lastNumber == 0));

        //! \todo Handle this better!
        if (i >= 1000)
        {
            return "<math>ERR</math>";
        }

        QString q = QString("<math><mfrac><mi>%1</mi><mn>%2</mn></mfrac></math>\n")
                    .arg(firstNumber.toString())
                    .arg(lastNumber.toString());

        return q;
#endif
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
        if (!integersOnly)
        {
            decimals = std::max(firstNumber.getDecimalPlaces(), lastNumber.getDecimalPlaces());
        }
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

#if 0
/*! Set maximum for the dividend.
 * \param newMax New maximum for the dividend.
 */
void DivisionModule::setFirstMaximum(BigFixedPoint newMax)
{
    if ((firstMax != newMax)
     || (firstMax.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        firstMax = newMax;
        QSettings settings;
        settings.setValue("divisionmodule/firstmax", firstMax.toString());

        //qDebug() << "First max (BFP) = " << firstMax.toString();
        mainWindow->newQuestion();
    }
}

/*! Set minimum for the dividend.
 * \param newMin New minimum for the dividend.
 */
void DivisionModule::setFirstMinimum(BigFixedPoint newMin)
{
    if ((firstMin != newMin)
     || (firstMin.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        firstMin = newMin;
        QSettings settings;
        settings.setValue("divisionmodule/firstmin", firstMin.toString());

        //qDebug() << "First min (BFP) = " << firstMin.toString();
        mainWindow->newQuestion();
    }
}

/*! Set maximum for the divisor.
 * \param newMax New maximum for the divisor.
 */
void DivisionModule::setLastMaximum(BigFixedPoint newMax)
{
    if ((lastMax != newMax)
     || (lastMax.getDecimalPlaces() != newMax.getDecimalPlaces()))
    {
        lastMax = newMax;
        QSettings settings;
        settings.setValue("divisionmodule/lastmax", lastMax.toString());

        //qDebug() << "Last max (BFP) = " << lastMax.toString();
        mainWindow->newQuestion();
    }
}

/*! Set minimum for the divisor.
 * \param newMin New minimum for the divisor.
 */
void DivisionModule::setLastMinimum(BigFixedPoint newMin)
{
    if ((lastMin != newMin)
     || (lastMin.getDecimalPlaces() != newMin.getDecimalPlaces()))
    {
        lastMin = newMin;
        QSettings settings;
        settings.setValue("divisionmodule/lastmin", lastMin.toString());

        //qDebug() << "Last min (BFP) = " << lastMin.toString();

        mainWindow->newQuestion();
    }
}

void DivisionModule::setLargestNumberFirst(bool b)
{
    if (this->largestNumberFirst != b)
    {
        this->largestNumberFirst = b;
        QSettings settings;
        settings.setValue("divisionmodule/largestNumberFirst", largestNumberFirst);

        mainWindow->newQuestion();
    }
}

void DivisionModule::setDecimalPlaces(int newDecimals)
{
    if (decimalPlaces != newDecimals)
    {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("divisionmodule/decimalplaces", decimalPlaces);

        mainWindow->newQuestion();
    }
}

void DivisionModule::setRoundingMode(bool rnd)
{
    if (roundingMode != rnd)
    {
        roundingMode = rnd;
        BigFixedPoint::setRounding(roundingMode);
        QSettings settings;
        settings.setValue("divisionmodule/roundingmode", roundingMode);

        mainWindow->newQuestion();
    }
}

void DivisionModule::setIntegersOnly(bool intsOnly)
{
    if (integersOnly != intsOnly)
    {
        integersOnly = intsOnly;
        QSettings settings;
        settings.setValue("divisionmodule/integersonly", integersOnly);

        if (integersOnly)
        {
            firstRangeUpdated();
        }
        mainWindow->newQuestion();
    }
}

void DivisionModule::setFirstMaximum(qint64 newMax)
{
    if (firstMaxIR != newMax)
    {
        firstMaxIR = newMax;
        QSettings settings;
        settings.setValue("divisionmodule/firstmax", QString("%1").arg(firstMaxIR));

        qDebug() << "New first max = " << firstMaxIR;
        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

/*! Set minimum for the dividend.
 * \param newMin New minimum for the dividend.
 */
void DivisionModule::setFirstMinimum(qint64 newMin)
{
    if (firstMinIR != newMin)
    {
        firstMinIR = newMin;
        QSettings settings;
        settings.setValue("divisionmodule/firstmin", QString("%1").arg(firstMinIR));

        qDebug() << "New first min = " << firstMinIR;
        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

/*! Set maximum for the divisor.
 * \param newMax New maximum for the divisor.
 */
void DivisionModule::setLastMaximum(qint64 newMax)
{
    if (lastMaxIR != newMax)
    {
        lastMaxIR = newMax;
        QSettings settings;
        settings.setValue("divisionmodule/lastmax", QString("%1").arg(lastMaxIR));

        qDebug() << "New last max = " << lastMaxIR;
        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

/*! Set minimum for the divisor.
 * \param newMin New minimum for the divisor.
 */
void DivisionModule::setLastMinimum(qint64 newMin)
{
    if (lastMinIR != newMin)
    {
        lastMinIR = newMin;
        QSettings settings;
        settings.setValue("divisionmodule/lastmin", QString("%1").arg(lastMinIR));

        qDebug() << "New last min = " << lastMinIR;
        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}
#endif
void DivisionModule::firstRangeUpdated()
{
    // Get rid of previous generator
    delete genFirst;
    genFirst = 0;

    // Make new generator
    qint64 minGen = std::min(firstMinIR, firstMaxIR);
    qint64 maxGen = std::max(firstMaxIR, firstMinIR);
    //qDebug() << "rnd: min = " << minGen << "; max = " << maxGen;

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
