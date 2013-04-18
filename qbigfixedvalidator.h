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
#ifndef QBIGFIXEDVALIDATOR_H
#define QBIGFIXEDVALIDATOR_H

#include <QValidator>
#include "bigfixedpoint.h"

class QBigFixedValidator : public QValidator
{
    Q_OBJECT
    //Q_PROPERTY(qulonglong bottom READ bottom WRITE setBottom)
    //Q_PROPERTY(qulonglong top READ top WRITE setTop)

public:
    explicit QBigFixedValidator(QObject *parent);
    QBigFixedValidator(BigFixedPoint bottom, BigFixedPoint top, QObject *parent);
    ~QBigFixedValidator();

    void setBottom(BigFixedPoint bottom);
    void setTop(BigFixedPoint top);
    virtual void setRange(BigFixedPoint bottom, BigFixedPoint top);
    void setRangeEnabled(bool enabled);

    BigFixedPoint bottom() const { return min; }
    BigFixedPoint top() const { return max; }

    QValidator::State validate(QString &input, int &) const;

private:
    bool rangeEnabled;
    BigFixedPoint min;
    BigFixedPoint max;

    Q_DISABLE_COPY(QBigFixedValidator)
};

#endif // QBIGFIXEDVALIDATOR_H
