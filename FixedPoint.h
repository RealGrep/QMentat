#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

#include <QDebug>

template <class inttype> class FixedPoint
{
public:
    FixedPoint(inttype num, int decimals)
        : number(num), decimalPlaces(decimals)
    { }

    void test() { qDebug() << "Decimals: " << decimalPlaces; }


    inttype getValue()
    {
        return number;
    }

    int getDecimalPlaces()
    {
        return decimalPlaces;
    }

protected:
    inttype number;
    int decimalPlaces;

};

template<typename inttype>
FixedPoint<inttype>
        operator+(FixedPoint<inttype>& x,
                  FixedPoint<inttype>& y)
{
    qDebug() << x.getDecimalPlaces();
    qDebug() << y.getDecimalPlaces();
    //qDebug() << decimalPlaces1;
    //qDebug() << decimalPlaces2;
    /*! \todo Handle scaling */
    FixedPoint<inttype> r(x.getValue() + y.getValue(), x.getDecimalPlaces());
    return r;
}


#endif // FIXEDPOINT_H
