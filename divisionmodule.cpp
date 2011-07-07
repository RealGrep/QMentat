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

    // Read config
    QSettings settings;
    settings.beginGroup("divisionmodule");
    roundingMode = settings.value("roundingmode", false).toBool();
    BigFixedPoint::setRounding(roundingMode == 1);
    firstMin = BigFixedPoint(settings.value("firstmin", 2).toString());
    firstMax = BigFixedPoint(settings.value("firstmax", 100).toString());
    lastMin = BigFixedPoint(settings.value("lastmin", 2).toString());
    lastMax = BigFixedPoint(settings.value("lastmax", 100).toString());
    largestNumberFirst = settings.value("largestNumberFirst", false).toBool();
    decimalPlaces = settings.value("decimalplaces", 0).toInt();
    integersOnly = settings.value("integersonly", false).toBool();
    settings.endGroup();

    // Make config frame
    configFrame = new DivisionConfigFrame();
    configFrame->setModule(this);
    configFrame->setFirstMinimum(firstMin.toString());
    configFrame->setFirstMaximum(firstMax.toString());
    configFrame->setLastMinimum(lastMin.toString());
    configFrame->setLastMaximum(lastMax.toString());
    configFrame->setLargestNumberFirst(largestNumberFirst);
    configFrame->setDecimalPlaces(decimalPlaces);
    configFrame->setRoundingMode((roundingMode == true) ? 1 : 0);
    configFrame->setIntegersOnly(integersOnly);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());
}

DivisionModule::~DivisionModule()
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

#if 0
/*! Tests to see if any divisors divide any of the possible dividends.
 *  \param firstMin Minimum of the dividend.
 *  \param firstMax Maximum of the dividend.
 *  \param lastMin Minimum of the divisor.
 *  \param lastMax Maximum of the divisor.
 *  \returns true if at least one of the possible divisors divides at
 *    least one of the possible dividends, false otherwise.
 */
bool DivisionModule::isRangeOk(int firstMin, int firstMax,
                               int lastMin, int lastMax)
{
    bool isOk = false;

    // We ignore firstMax because we don't care if it's only divisible by itself
    for (int i = firstMax-1; i >= firstMin; --i)
    {
        for (int j = lastMin; j < lastMax; ++j)
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
std::vector<unsigned long> *DivisionModule::getDivisors(unsigned long num,
                                                        unsigned long min,
                                                        unsigned long max)
{
    std::vector<unsigned long> *divisors = new std::vector<unsigned long>();
    //std::cout << num << " is divisible by ";
    // No use going past num/2
    unsigned long last = std::min(num/2, max) + 1;
    //std::cout << "[" << min << "->" << last << "] ";
    for (unsigned long i = min; i < last; ++i)
    {
        if ((num % i) == 0)
        {
            //isOk = true;
            //std::cout << i << " ";
            divisors->push_back(i);
        }
    }
    //std::cout << std::endl;

    return divisors;
}
#endif

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

/*! \returns QString containing new question in MathML format.
 */
QString DivisionModule::question()
{
    if (integersOnly)
    {
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
                int random = qrand() % divisors->size();
                lastNumber = (*divisors)[random];
                /*
                for (size_t i = 0; i < divisors->size(); ++i)
                {
                    delete (*divisors)[i];
                }
                */
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
        firstNumberScaled.scale(firstNumber.getDecimalPlaces()+lastNumber.getDecimalPlaces()+decimalPlaces);
        //BigFixedPoint lastNumberScaled = lastNumber;
        //lastNumberScaled.scale(lastNumber.getDecimalPlaces()+decimalPlaces);
        answer = firstNumberScaled / lastNumber;
        assert(decimalPlaces >= 0);
        //assert(decimalPlaces <= answer.getDecimalPlaces());
        answer.scale(decimalPlaces);
        //std::cout << "first = " << firstNumber.toString().toStdString() << std::endl;
        //std::cout << "last = " << lastNumber.toString().toStdString() << std::endl;

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
#if 0
    assert(genFirst != 0);
    assert(genLast != 0);
    //bool bIntegerResults = true;
    //bool trivial = false;
    //int attempts = 0;

    // Check range
    // TODO: Fix it so it can never happen
    bool rangeOk = isRangeOk(this->firstMin, this->firstMax,
                             this->lastMin, this->lastMax);
    if (!rangeOk) return "<math>ERR</math>";

    // Ensure we don't get a division by 0
    do {
        // Generate the numbers
        firstNumber = (*genFirst)();

        std::vector<unsigned long> *divisors = getDivisors(this->firstNumber,
                                                           this->lastMin, this->lastMax);
        /*
        std::cout << "Divisors for " << this->firstNumber << ": ";
        for (auto it = divisors->begin(); it != divisors->end(); ++it)
        {
            std::cout << (*it) << " ";
        }
        std::cout << std::endl;
        */
        if (divisors->empty()) {
            // No divisors, try again
            //std::cout << "No divisors!" << std::endl;
            this->lastNumber = 0;
            delete divisors;
            continue;
        } else {
            int random = qrand() % divisors->size();
            this->lastNumber = (int)(*divisors)[random];
            delete divisors;
        }

        /*
        this->lastNumber = (qrand() % (this->lastMax - this->lastMin + 1)) + this->lastMin;
        if (this->largestNumberFirst && (this->lastNumber > this->firstNumber))
        {
            int tmp = this->firstNumber;
            this->firstNumber = this->lastNumber;
            this->lastNumber = tmp;
        }
        */
        this->answer = static_cast<quint64>(firstNumber)
                       / static_cast<quint64>(lastNumber);

        /*
        trivial = false;
        if (this->firstNumber == this->lastNumber) {
            trivial = true;
        }
        ++attempts;
        */
    } while (this->lastNumber == 0);

    QString q = QString("<math><mfrac><mi>%1</mi><mn>%2</mn></mfrac></math>\n")
                .arg(decimalize(firstNumber, decimalPlaces))
                .arg(decimalize(lastNumber, decimalPlaces));

    return q;
#endif
}

/*! Tells if the supplied answer is correct.
 * \returns true if correct, false otherwise.
 */
bool DivisionModule::isCorrect(QString& answerGiven)
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

/*! Produces answer string for current question.
 * \returns QString containing full answer for display.
 */
QString DivisionModule::getAnswerString()
{
    return QString("%1 / %2 = %3")
            .arg(firstNumber.toString())
            .arg(lastNumber.toString())
            .arg(answer.toString());
}

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

        mainWindow->newQuestion();
    }
}
