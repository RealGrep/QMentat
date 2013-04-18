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
#ifndef QULONGLONGVALIDATOR_H
#define QULONGLONGVALIDATOR_H

#include <QValidator>

class QULongLongValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(qulonglong bottom READ bottom WRITE setBottom)
            Q_PROPERTY(qulonglong top READ top WRITE setTop)

public:
            explicit QULongLongValidator(QObject *parent);
    QULongLongValidator(qulonglong bottom, qulonglong top, QObject *parent);
    ~QULongLongValidator();

    void setBottom(qulonglong bottom);
    void setTop(qulonglong top);
    virtual void setRange(qulonglong  bottom, qulonglong top);

    qulonglong bottom() const { return min; }
    qulonglong top() const { return max; }

    QValidator::State validate(QString &input, int &) const;

private:
    Q_DISABLE_COPY(QULongLongValidator)

    qulonglong min;
    qulonglong max;
};

#endif // QULONGLONGVALIDATOR_H
