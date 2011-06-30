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

/*! \class DivisionModule
 *  \brief Handles logic of questions and answers for division practice.
 *  \author Mike Dusseault <mike.dusseault@gmail.com>
 */
DivisionModule::DivisionModule(MainWindow *mw)
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
    settings.beginGroup("divisionmodule");
    firstMin = settings.value("firstmin", 2).toULongLong();
    firstMax = settings.value("firstmax", 100).toULongLong();
    lastMin = settings.value("lastmin", 2).toULongLong();
    lastMax = settings.value("lastmax", 100).toULongLong();
    largestNumberFirst = settings.value("largestNumberFirst", false).toBool();
    decimalPlaces = settings.value("decimalplaces", 0).toUInt();
    settings.endGroup();

    // Make config frame
    configFrame = new DivisionConfigFrame();
    configFrame->setModule(this);
    configFrame->setFirstMinimum(firstMin);
    configFrame->setFirstMaximum(firstMax);
    configFrame->setLastMinimum(lastMin);
    configFrame->setLastMaximum(lastMax);
    configFrame->setLargestNumberFirst(largestNumberFirst);
    configFrame->setDecimalPlaces(decimalPlaces);

    // Make display frame
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());

    firstRangeUpdated();
    lastRangeUpdated();
}

DivisionModule::~DivisionModule()
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

/*! \returns QString containing new question in MathML format.
 */
QString DivisionModule::question()
{
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
 //       lastNumber = (*genLast)();

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
}

/*! Tells if the supplied answer is correct.
 * \returns true if correct, false otherwise.
 */
bool DivisionModule::isCorrect(QString& answerGiven)
{
    // Figure out how many decimal places we're "missing" and get them back
    // so the comparison works right
    int decimalPos = answerGiven.indexOf(QLocale::system().decimalPoint());
    quint64 answerNum = 0;
    if (decimalPos >= 0)
    {
        int missingDecimals = decimalPlaces/2 - (answerGiven.size() - 1
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

    qDebug() << "isCorrect: answerGiven = " << answerNum << "; answer = " << answer;
    if (answerNum == this->answer)
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
            .arg(decimalize(firstNumber, decimalPlaces))
            .arg(decimalize(lastNumber, decimalPlaces))
            .arg(decimalize(answer, decimalPlaces/2));
}

void DivisionModule::firstRangeUpdated()
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
void DivisionModule::lastRangeUpdated()
{
    // Get rid of previous generator
    delete genLast;
    genLast = 0;

    // Make new generator
    quint64 min = lastMin * static_cast<quint64>(pow(10, decimalPlaces));
    quint64 max = lastMax * static_cast<quint64>(pow(10, decimalPlaces));

    genLast = new RandomInt<quint64>(min, max);
}

/*! Set maximum for the dividend.
 * \param newMax New maximum for the dividend.
 */
void DivisionModule::setFirstMaximum(quint64 newMax)
{
    if (this->firstMax != newMax) {
        this->firstMax = newMax;
        QSettings settings;
        settings.setValue("divisionmodule/firstmax", firstMax);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

/*! Set minimum for the dividend.
 * \param newMin New minimum for the dividend.
 */
void DivisionModule::setFirstMinimum(quint64 newMin)
{
    if (this->firstMin != newMin) {
        this->firstMin = newMin;
        QSettings settings;
        settings.setValue("divisionmodule/firstmin", firstMin);

        firstRangeUpdated();
        mainWindow->newQuestion();
    }
}

/*! Set maximum for the divisor.
 * \param newMax New maximum for the divisor.
 */
void DivisionModule::setLastMaximum(quint64 newMax)
{
    if (this->lastMax != newMax) {
        this->lastMax = newMax;
        QSettings settings;
        settings.setValue("divisionmodule/lastmax", lastMax);

        lastRangeUpdated();
        mainWindow->newQuestion();
    }
}

/*! Set minimum for the divisor.
 * \param newMin New minimum for the divisor.
 */
void DivisionModule::setLastMinimum(quint64 newMin)
{
    if (this->lastMin != newMin) {
        this->lastMin = newMin;
        QSettings settings;
        settings.setValue("divisionmodule/lastmin", lastMin);

        lastRangeUpdated();
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

        firstRangeUpdated();
        lastRangeUpdated();
        mainWindow->newQuestion();
    }
}

void DivisionModule::setDecimalPlaces(quint32 newDecimals)
{
    if (decimalPlaces != newDecimals) {
        decimalPlaces = newDecimals;
        QSettings settings;
        settings.setValue("divisionmodule/decimalplaces", decimalPlaces);

        firstRangeUpdated();
        lastRangeUpdated();
        mainWindow->newQuestion();
    }
}
