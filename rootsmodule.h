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
#ifndef ROOTSMODULE_H
#define ROOTSMODULE_H

#include "practicemodule.h"

#include "bigfixedpoint.h"

// Forward declarations
template <class inttype> class RandomInt;
class RootsConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class RootsModule : public PracticeModule
{
public:
    RootsModule(MainWindow *mw);
    ~RootsModule();

    QString question();
    bool isCorrect(const QString& answerGiven);
    RootsConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    bool isRangeOk(const BigFixedPoint& newMin, const BigFixedPoint& newMax,
                   int newRootMin, int newRootMax);

    bool applyConfig();

    void setSettings(const BigFixedPoint& newFirstMin,
                     const BigFixedPoint& newFirstMax,
                     int newRootMin,
                     int newRootMax,
                     bool newIntsOnly,
                     int newDecimals,
                     bool newRoundingMode);

private:
    MainWindow *mainWindow;

    BigFixedPoint min;
    BigFixedPoint max;
    int rootMin;
    int rootMax;
    BigFixedPoint firstNumber;
    int root;
    BigFixedPoint answer;
    int decimalPlaces;
    bool roundingMode;
    bool integersOnly;

    RandomInt<int> *genRoot;

    void rootRangeUpdated();
};

#endif // ROOTSMODULE_H
