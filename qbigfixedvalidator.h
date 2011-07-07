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

    BigFixedPoint bottom() const { return min; }
    BigFixedPoint top() const { return max; }

    QValidator::State validate(QString &input, int &) const;

private:
    BigFixedPoint min;
    BigFixedPoint max;

    Q_DISABLE_COPY(QBigFixedValidator)
};

#endif // QBIGFIXEDVALIDATOR_H
