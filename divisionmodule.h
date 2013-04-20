/* Copyright (c) 2013 Mike Dusseault
 *
 * This file is part of QMentat.
 *
 * QMentat is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QMentat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QMentat.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DIVISIONMODULE_H
#define DIVISIONMODULE_H

#include "practicemodule.h"
#include "bigfixedpoint.h"
#include <vector>

// Forward declarations
template <class inttype> class RandomInt;
class DivisionConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class DivisionModule : public PracticeModule
{
public:
    DivisionModule(MainWindow *mw);
    ~DivisionModule();

    QString question();
    bool isCorrect(const QString& answerGiven);
    DivisionConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    bool applyConfig();

    // For fixed point results
    void setSettings(const BigFixedPoint& newFirstMin,
                     const BigFixedPoint& newFirstMax,
                     const BigFixedPoint& newLastMin,
                     const BigFixedPoint& newLastMax,
                     bool newLargestNumberFirst, bool newIntsOnly,
                     int newDecimals, bool newRoundingMode);

    // For integer results only mode
    void setSettings(qint64 newFirstMin, qint64 newFirstMax,
                     qint64 newLastMin, qint64 newLastMax,
                     bool newlargestNumberFirst, bool newIntsOnly,
                     int newDecimals, bool newRoundingMode);
    bool isRangeOk(qint64 firstMin, qint64 firstMax,
                   qint64 lastMin, qint64 lastMax) const;

    bool isIntegersOnly() const
    {
        return integersOnly;
    }

private:
    MainWindow *mainWindow;

    // In BigFixedPoint mode (not Integer Results Only)
    BigFixedPoint firstMin;
    BigFixedPoint firstMax;
    BigFixedPoint lastMin;
    BigFixedPoint lastMax;
    BigFixedPoint firstNumber;
    BigFixedPoint lastNumber;
    BigFixedPoint answer;
    bool largestNumberFirst;
    int decimalPlaces;
    bool roundingMode;
    bool integersOnly;

    // In Integer Results Only mode
    qint64 firstMinIR;
    qint64 firstMaxIR;
    qint64 lastMinIR;
    qint64 lastMaxIR;
    qint64 firstNumberIR;
    qint64 lastNumberIR;
    qint64 answerIR;

    RandomInt<qint64> *genFirst;

    void firstRangeUpdated();

    //std::vector<BigFixedPoint> *getDivisors(BigFixedPoint& num,
    //                                        BigFixedPoint& min,
    //                                        BigFixedPoint& max);

    std::vector<qint64> *getDivisors(qint64 num, qint64 min, qint64 max) const;
};

#endif // DIVISIONMODULE_H
