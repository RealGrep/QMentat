#include "bigfixedpoint.h"
#include <QtGui>
#include <gmpxx.h>
#include <iostream>
#include <string>

BigFixedPoint::BigFixedPoint(const BigFixedPoint& bfp)
{
    if (this == &bfp)
        return;
    number = bfp.number;
    decimalPlaces = bfp.decimalPlaces;
}

BigFixedPoint::BigFixedPoint(std::string num, int decimals)
    : number(num), decimalPlaces(decimals)
{
    // Empty
}

BigFixedPoint::BigFixedPoint(mpz_class num, int decimals)
    : number(num), decimalPlaces(decimals)
{
    // Empty
}

BigFixedPoint::BigFixedPoint(std::string num)
{
    // Strip separators out
    for (size_t i = 0; i < num.size(); ++i)
    {
        if (num[i] == QLocale::system().groupSeparator())
        {
            num.erase(i, 1);
        }
    }

    // Where's the decimal point?
    int decimalLoc = num.find(QLocale::system().decimalPoint().toAscii());
    if (decimalLoc <= 0)
    {
        decimalPlaces = 0;
    } else {
        decimalPlaces = num.size()-1 - decimalLoc;
        num.erase(decimalLoc, 1);
    }

    // The number string is clean
    number = num;
}

BigFixedPoint::~BigFixedPoint()
{

}

void BigFixedPoint::scale(int decimals)
{
    int adjustment = decimals - decimalPlaces;
    mpz_class factor;
    mpz_ui_pow_ui(factor.get_mpz_t(), 10, adjustment);
    mpz_class adjusted = number * factor;
    number = adjusted;
    decimalPlaces = decimals;
}

BigFixedPoint& BigFixedPoint::operator=(const BigFixedPoint &rhs)
{
    // Check for self-assignment
    if (this == &rhs)
        return *this;

    number = rhs.number;
    decimalPlaces = rhs.decimalPlaces;
    return *this;
}


bool BigFixedPoint::operator==(const BigFixedPoint &rhs) const {
    BigFixedPoint lhsScaled = *this;
    BigFixedPoint rhsScaled = rhs;

    if (lhsScaled.decimalPlaces > rhs.decimalPlaces)
    {
        // Scale rhs up to lhs
        rhsScaled.scale(lhsScaled.decimalPlaces);
    } else if (lhsScaled.decimalPlaces < rhs.decimalPlaces) {
        lhsScaled.scale(rhsScaled.decimalPlaces);
    }

    if ((lhsScaled.decimalPlaces == rhsScaled.decimalPlaces)
        && (lhsScaled.number == rhsScaled.number))
    {
        return true;
    } else {
        return false;
    }
}

bool BigFixedPoint::operator!=(const BigFixedPoint &rhs) const {
  return !(*this == rhs);
}

bool BigFixedPoint::operator>(const BigFixedPoint &rhs) const {
    BigFixedPoint lhsScaled = *this;
    BigFixedPoint rhsScaled = rhs;
    if (lhsScaled.decimalPlaces > rhs.decimalPlaces)
    {
        // Scale rhs up to lhs
        rhsScaled.scale(lhsScaled.decimalPlaces);
    } else if (lhsScaled.decimalPlaces < rhs.decimalPlaces) {
        lhsScaled.scale(rhsScaled.decimalPlaces);
    }

    return lhsScaled.number > rhsScaled.number;
}

bool BigFixedPoint::operator<=(const BigFixedPoint &rhs) const {
    return !(*this > rhs);
}

bool BigFixedPoint::operator<(const BigFixedPoint &rhs) const {
    BigFixedPoint lhsScaled = *this;
    BigFixedPoint rhsScaled = rhs;
    if (lhsScaled.decimalPlaces > rhs.decimalPlaces)
    {
        // Scale rhs up to lhs
        rhsScaled.scale(lhsScaled.decimalPlaces);
    } else if (lhsScaled.decimalPlaces < rhs.decimalPlaces) {
        lhsScaled.scale(rhsScaled.decimalPlaces);
    }

    return lhsScaled.number < rhsScaled.number;
}

bool BigFixedPoint::operator>=(const BigFixedPoint &rhs) const {
    return !(*this < rhs);
}

BigFixedPoint& BigFixedPoint::operator+=(const BigFixedPoint& y) {
    int decimals = decimalPlaces;
    if (decimals > y.getDecimalPlaces())
    {
        int adjustment = decimals - y.getDecimalPlaces();
        mpz_class factor;
        mpz_ui_pow_ui(factor.get_mpz_t(), 10, adjustment);
        number = number + y.getValue()*factor;
    } else if (decimals < y.getDecimalPlaces()) {
        decimals = y.getDecimalPlaces();
        int adjustment = y.decimalPlaces - decimalPlaces;
        mpz_class factor;
        mpz_ui_pow_ui(factor.get_mpz_t(), 10, adjustment);
        number = number*factor + y.getValue();
        decimalPlaces = decimals;
    } else {
        number += y.getValue();
    }

    return *this;
}

const BigFixedPoint BigFixedPoint::operator+(const BigFixedPoint& y) const
{
    return BigFixedPoint(*this) += y;
}

BigFixedPoint& BigFixedPoint::operator-=(const BigFixedPoint& y) {
    int decimals = decimalPlaces;
    // Scale parameter to our scale
    if (decimals > y.getDecimalPlaces())
    {
        int adjustment = decimals - y.getDecimalPlaces();
        mpz_class factor;
        mpz_ui_pow_ui(factor.get_mpz_t(), 10, adjustment);
        number = number - y.getValue()*factor;
    } else if (decimals < y.getDecimalPlaces()) {
        // Scale ourselves to the parameter
        decimals = y.getDecimalPlaces();
        int adjustment = y.decimalPlaces - decimalPlaces;
        mpz_class factor;
        mpz_ui_pow_ui(factor.get_mpz_t(), 10, adjustment);
        number = number*factor - y.getValue();
        decimalPlaces = decimals;
    } else {
        number -= y.getValue();
    }

    return *this;
}

const BigFixedPoint BigFixedPoint::operator-(const BigFixedPoint& y) const
{
    return BigFixedPoint(*this) -= y;
}

BigFixedPoint& BigFixedPoint::operator*=(const BigFixedPoint& y)
                         {
    int decimals = decimalPlaces + y.getDecimalPlaces();
    number = number*y.getValue();
    decimalPlaces = decimals;
    return *this;
}

const BigFixedPoint BigFixedPoint::operator*(const BigFixedPoint& y) const
{
    return BigFixedPoint(*this) *= y;
}

BigFixedPoint& BigFixedPoint::operator/=(const BigFixedPoint& y)
{
    //!\todo Scale or something to not lose precision
    assert(y.getDecimalPlaces() <= decimalPlaces);
    int decimals = decimalPlaces - y.getDecimalPlaces();
    number = number/y.getValue();
    decimalPlaces = decimals;
    return *this;
}

const BigFixedPoint BigFixedPoint::operator/(const BigFixedPoint& y) const
{
    return BigFixedPoint(*this) /= y;
}


BigFixedPoint BigFixedPoint::random(const BigFixedPoint& min, const BigFixedPoint& max)
{
    BigFixedPoint range = max - min;
    return BigFixedPoint::random(range) + min;
}

BigFixedPoint BigFixedPoint::random(const BigFixedPoint& n)
{
    gmp_randclass r(gmp_randinit_default);;
    r.seed(QTime::currentTime().msec());
    mpz_class num = r.get_z_range(n.number);
    return BigFixedPoint(num, n.getDecimalPlaces());
}

QString BigFixedPoint::toString() const
{
    // Handle this better. Need to handle it mostly in the division
    // code, where this can occur if we end up with a scaling factor
    // of 10^x where x < 0. Maybe here we should assume 0, since it
    // sort of "underflowed". If we have adaptive precision in division
    // we can get around the loss of precision in the first place.
    assert(decimalPlaces >= 0);

    //! \todo Add a single 0 if there's no digits before decimal

    QString str;
    if (decimalPlaces == 0)
    {
        str = QString(number.get_str().c_str());
    } else {
        QString numStr = QString(number.get_str().c_str());

        // Add back leading 0's
        if (numStr.size() < (decimalPlaces+1))
        {
            int addNum = decimalPlaces + 1 - numStr.size();
            //std::cout << "Adding " << addNum << " leading 0s." << std::endl;
            for (int i = 0; i < addNum; ++i)
            {
                numStr.insert(0, QLocale::system().zeroDigit().toAscii());
            }
        }

        int decimalLoc = numStr.size() - decimalPlaces;
        QString integerPart = numStr.left(decimalLoc);
        QString fractionalPart = numStr.right(decimalPlaces);

        // Insert decimal separator into the integer part
        int partSize = integerPart.size();
        for (int i = 0; i < partSize-1; ++i)
        {
            if ((i > 0) && ((i % 3) == 0))
            {
                int pos = (partSize) - i;
                //std::cout << "i = " << i << "; pos = " << pos << "; len = " << integerPart.size() << std::endl;
                integerPart.insert(pos, QLocale::system().groupSeparator());
                //std::cout << integerPart.toStdString() << std::endl;
            }
        }

        // Insert decimal separator into the fractional part
        if (fractionalPart.size() > 4) {
            for (int i = 0; i < decimalPlaces; ++i)
            {
                int pos = i + (i/3)-1;
                //std::cout << "i = " << i << "; pos = " << pos << std::endl;
                if ((i > 0) && ((i % 3) == 0))
                {
                    //std::cout << "Inserting separator" << std::endl;
                    fractionalPart.insert(pos, QLocale::system().groupSeparator());
                }
            }
        }

        str = QString("%1%2%3")
              .arg(integerPart)
              .arg(QLocale::system().decimalPoint())
              .arg(fractionalPart);
    }

    return str;
}
