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
