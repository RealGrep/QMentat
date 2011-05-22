#ifndef PRACTICEMODULE_H
#define PRACTICEMODULE_H

#include <QtGui>
#include "questiondisplay.h"

class PracticeModule
{
public:
    //PracticeModule(MainWindow *mw);
    virtual QString question() = 0;
    virtual bool isCorrect(QString& answerGiven) = 0;
    virtual QFrame* getConfigFrame() = 0;
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

          str = QString("%L1.%L2")
                .arg(integerPart)
                .arg(fractionalPart, decimals, 10, QChar('0'));
       }

       return str;
    }


};

#endif // PRACTICEMODULE_H
