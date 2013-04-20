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
#ifndef POWERSMODULE_H
#define POWERSMODULE_H

#include "practicemodule.h"

#include "bigfixedpoint.h"

// Forward declarations
template <class inttype> class RandomInt;
class PowersConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class PowersModule : public PracticeModule
{
public:
    PowersModule(MainWindow *mw);
    ~PowersModule();

    QString question();
    bool isCorrect(const QString& answerGiven);
    PowersConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    bool applyConfig();

    void setSettings(const BigFixedPoint& newFirstMin,
                     const BigFixedPoint& newFirstMax,
                     int newPowerMin,
                     int newPowerMax,
                     int newDecimals,
                     bool newRoundingMode);

private:
    MainWindow *mainWindow;

    BigFixedPoint min;
    BigFixedPoint max;
    int powerMin;
    int powerMax;
    BigFixedPoint firstNumber;
    int power;
    BigFixedPoint answer;
    int decimalPlaces;
    bool roundingMode;

    RandomInt<int> *genPower;

    void powerRangeUpdated();
};

#endif // POWERSMODULE_H
