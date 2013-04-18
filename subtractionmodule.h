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
#ifndef SUBTRACTIONMODULE_H
#define SUBTRACTIONMODULE_H

#include "practicemodule.h"
#include "bigfixedpoint.h"

// Forward declarations
template <class inttype> class RandomInt;
class SubtractionConfigFrame;
class MainWindow;
class QuestionDisplay;
class QString;
class QFrame;

class SubtractionModule : public PracticeModule
{
public:
    SubtractionModule(MainWindow *mw);
    ~SubtractionModule();

    QString question();
    bool isCorrect(QString& answerGiven);
    SubtractionConfigFrame *configFrame;
    QFrame* getConfigFrame();
    QuestionDisplay *displayFrame;
    QuestionDisplay* getDisplayFrame();
    QString getAnswerString();

    bool applyConfig();

    void setSettings(BigFixedPoint newFirstMin,
                     BigFixedPoint newFirstMax,
                     BigFixedPoint newLastMin,
                     BigFixedPoint newLastMax,
                     bool newLargestNumberFirst);

private:
    MainWindow *mainWindow;

    BigFixedPoint firstMin;
    BigFixedPoint firstMax;
    BigFixedPoint lastMin;
    BigFixedPoint lastMax;
    BigFixedPoint firstNumber;
    BigFixedPoint lastNumber;
    BigFixedPoint answer;
    bool largestNumberFirst;
};

#endif // SUBTRACTIONMODULE_H
