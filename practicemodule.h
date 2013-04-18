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
#ifndef PRACTICEMODULE_H
#define PRACTICEMODULE_H

#include <QtGui>
#include "questiondisplay.h"

class PracticeModule
{
public:
    //PracticeModule(MainWindow *mw);
    virtual ~PracticeModule() { }
    virtual QString question() = 0;
    virtual bool isCorrect(QString& answerGiven) = 0;
    virtual QFrame* getConfigFrame() = 0;
    virtual bool applyConfig() = 0;
    virtual QuestionDisplay* getDisplayFrame() = 0;
    virtual QString getAnswerString() = 0;

    static QString decimalize(quint64 num, quint32 decimals)
    {
        QString str;
        if (decimals == 0)
        {
            str = QString("%L1").arg(num);
        } else {
            QString numStr = QString("%1").arg(num);
            int decimalLoc = numStr.size() - decimals;
            quint64 integerPart = numStr.left(decimalLoc).toULongLong();
            quint64 fractionalPart = numStr.right(decimals).toULongLong();
            //qDebug() << "numStr = " << numStr;
            //qDebug() << "integer = " << integerPart;
            //qDebug() << "fract = " << fractionalPart;

            str = QString("%L1%2%3")
                  .arg(integerPart)
                  .arg(QLocale::system().decimalPoint())
                  .arg(fractionalPart, decimals, 10, QLocale::system().zeroDigit());
            // Insert decimal separator into the fractional part
            if (decimals > 4) {
                for (size_t i = 0; i < decimals; ++i)
                {
                    size_t pos = i + (str.size() - decimals);
                    //qDebug() << "i = " << i << "; pos = " << pos;
                    if ((i > 0) && ((i % 3) == 0))
                    {
                        //qDebug() << "Inserting separator";
                        str.insert(pos, QLocale::system().groupSeparator());
                    }
                }
            }
        }

        return str;
    }


};

#endif // PRACTICEMODULE_H
