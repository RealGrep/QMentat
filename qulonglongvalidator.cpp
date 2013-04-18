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
#include "qulonglongvalidator.h"

QULongLongValidator::QULongLongValidator(QObject *parent)
    : QValidator(parent),
    min(0), max(Q_UINT64_C(0xFFFFFFFFFFFFFFFF))
{
    // Empty
}

QULongLongValidator::QULongLongValidator(qulonglong bottom, qulonglong top,
                                         QObject *parent)
                                             : QValidator(parent), min(bottom), max(top)
{
    // Empty
}

QULongLongValidator::~QULongLongValidator()
{
    // Empty
}

void QULongLongValidator::setRange(qulonglong bottom, qulonglong top)
{
    min = bottom;
    max = top;
}

void QULongLongValidator::setBottom(qulonglong bottom)
{
    setRange(bottom, top());
}

void QULongLongValidator::setTop(qulonglong top)
{
    setRange(bottom(), top);
}

QValidator::State QULongLongValidator::validate(QString &input, int &) const
{
    if (input.isEmpty())
    {
        return Intermediate;
    }

    bool ok;
    qulonglong entered = input.toULongLong(&ok);
    if (input.contains(QLatin1Char(' ')) || input.contains(QLatin1Char('-')) || !ok)
    {
        return Invalid;
    }

    if (entered >= min && entered <= max)
    {
        return Acceptable;
    }

    return Invalid;
}
