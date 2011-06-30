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
