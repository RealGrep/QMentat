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
#include "qbigfixedvalidator.h"

#include <QtGui>
#include "bigfixedpoint.h"

QBigFixedValidator::QBigFixedValidator(QObject *parent)
    : QValidator(parent),
    rangeEnabled(false),
    min(BigFixedPoint(QString("0"))),
    max(BigFixedPoint(QString("99999999999999999999999999999999999999999999")))
{
    // Empty
}

QBigFixedValidator::QBigFixedValidator(BigFixedPoint bottom, BigFixedPoint top,
                                       QObject *parent)
    : QValidator(parent), rangeEnabled(true), min(bottom), max(top)
{
    // Empty
}

QBigFixedValidator::~QBigFixedValidator()
{
    // Empty
}

void QBigFixedValidator::setRangeEnabled(bool enabled)
{
    rangeEnabled = enabled;
}

void QBigFixedValidator::setRange(BigFixedPoint bottom, BigFixedPoint top)
{
    rangeEnabled = true;
    min = bottom;
    max = top;
}

void QBigFixedValidator::setBottom(BigFixedPoint bottom)
{
    setRange(bottom, top());
}

void QBigFixedValidator::setTop(BigFixedPoint top)
{
    setRange(bottom(), top);
}

QValidator::State QBigFixedValidator::validate(QString &input, int &) const
{
    // A negative sign, when the range is enabled, is not allowed if the
    // range minimum is equal to or greather than 0.
    if (input.size() == 1 && input[0] == QLocale::system().negativeSign()
            && (rangeEnabled && min >= 0))
    {
        return Invalid;
    }

    // A single negative sign with no digits should still be allowed as an
    // intermediate step, even though it's fails the BigFixedPoint validity
    // test.
    if (input.isEmpty()
        || (input.size() == 1 && input[0] == QLocale::system().negativeSign()))
    {
        //qDebug() << "Intermediate";
        return Intermediate;
    }

    if (input.contains(QLatin1Char(' ')) || !BigFixedPoint::isValid(input))
    {
        //qDebug() << "Invalid";
        return Invalid;
    }

    //qDebug() << "Entered: " << input;
    if (rangeEnabled)
    {
        BigFixedPoint entered(input);
        if (entered >= min && entered <= max)
        {
            return Acceptable;
        }
    } else {
        return Acceptable;
    }

    return Invalid;
}
