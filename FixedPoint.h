#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

#include <iostream>
using std::ostream;

#include <QDebug>
// REMOVE ME WHEN DONE TESTING
#include "practicemodule.h"

template <class inttype> class FixedPoint;

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

    FixedPoint<inttype> scale(int decimals)
    {
        int adjustment = decimals - decimalPlaces;
        inttype adjusted = number * exp10(adjustment);
        FixedPoint<inttype> r(adjusted, decimals);
        return r;
    }

    friend std::ostream&
            operator<< (std::ostream &os, const FixedPoint<inttype> &x)
    {
        os << PracticeModule::decimalize(x.number, x.decimalPlaces).toStdString();

        return os;
    }


protected:
    inttype number;
    int decimalPlaces;

};

template<typename inttype> FixedPoint<inttype>
        operator+(FixedPoint<inttype>& x,
                  FixedPoint<inttype>& y)
{

    int decimals = x.getDecimalPlaces();
    if (decimals > y.getDecimalPlaces())
    {
        int adjustment = decimals - y.getDecimalPlaces();
        FixedPoint<inttype> r(x.getValue() + y.getValue()*(inttype)exp10(adjustment), decimals);
        return r;
    } else if (decimals < y.getDecimalPlaces()) {
        decimals = y.getDecimalPlaces();
        int adjustment = decimals-x.getDecimalPlaces();
        FixedPoint<inttype> r(x.getValue()*(inttype)exp10(adjustment) + y.getValue(), decimals);
        return r;
    } else {
        FixedPoint<inttype> r(x.getValue() + y.getValue(),
                              decimals);
        return r;
    }
}

template<typename inttype> FixedPoint<inttype>
        operator-(FixedPoint<inttype>& x, FixedPoint<inttype>& y)
{
    int decimals = x.getDecimalPlaces();
    if (decimals > y.getDecimalPlaces())
    {
        int adjustment = decimals - y.getDecimalPlaces();
        FixedPoint<inttype> r(x.getValue() - y.getValue()*(inttype)exp10(adjustment), decimals);
        return r;
    } else if (decimals < y.getDecimalPlaces()) {
        decimals = y.getDecimalPlaces();
        int adjustment = decimals - x.getDecimalPlaces();
        FixedPoint<inttype> r(x.getValue()*(inttype)exp10(adjustment) - y.getValue(), decimals);
        return r;
    } else {
        FixedPoint<inttype> r(x.getValue() - y.getValue(),
                              decimals);
        return r;
    }
}

template<typename inttype> FixedPoint<inttype>
        operator*(FixedPoint<inttype>& x, FixedPoint<inttype>& y)
{
    int decimals = x.getDecimalPlaces() + y.getDecimalPlaces();
    FixedPoint<inttype> r(x.getValue()*y.getValue(), decimals);
    return r;
}

template<typename inttype> FixedPoint<inttype>
        operator/(FixedPoint<inttype>& x, FixedPoint<inttype>& y)
{
    int decimals = x.getDecimalPlaces() - y.getDecimalPlaces();
    FixedPoint<inttype> r(x.getValue()/y.getValue(), decimals);
    return r;
}

template <typename inttype> inline inttype rounded_division(inttype n, inttype d)
{
    inttype q = n / d;
    inttype r = n % d;

    if (n < 0)
    {
        // num -, den -, quot +, rem -
        if (d < 0)
        {
            if ((r << 1) < d)
            {
                ++q;
            }
        }
        // d >= 0)
        else
        {// num -, den +, quot -, rem -
            if ((-r << 1) > d)
            {
                --q;
            }
        }
    } else { // n >= 0
        if (d < 0)
        {// num +, den -, quot -, rem +
            if ((-r << 1) < d)
            {
                --q;
            }
        }
        else // d >= 0
        {// num +, den +, quot +, rem +
            if ((r << 1) > d)
            {
                ++q;
            }
        }
    }
    return q;
}
/*
template <int N> struct exp10
{
    enum { value = 10 * exp10<N-1>::value };
};

template <> struct exp10<0>
{
    enum { value = 1 };
};
*/

#endif // FIXEDPOINT_H
