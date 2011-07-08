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
    : QValidator(parent), min(bottom), max(top)
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
