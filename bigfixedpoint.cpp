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
#include "bigfixedpoint.h"
#include <QtGui>
#include <gmpxx.h>
#include <iostream>
#include <string>
#include <limits>

// For seeding gmp
#if defined(Q_OS_LINUX)
#   include <fstream>   // Linux only - for reading urandom
#else
#   include <QDateTime> // Use this for seeding on non-Linux systems
#endif

bool BigFixedPoint::roundingEnabled = false;

BigFixedPoint::BigFixedPoint()
{
    number = 0;
    decimalPlaces = 0;
}

BigFixedPoint::BigFixedPoint(const BigFixedPoint& bfp)
{
    number = bfp.number;
    decimalPlaces = bfp.decimalPlaces;
}

BigFixedPoint::BigFixedPoint(int num)
    : number(num), decimalPlaces(0)
{
    // Empty
}

BigFixedPoint::BigFixedPoint(qint64 num)
    : decimalPlaces(0)
{
    QString numStr = QString("%1").arg(num);
    number = numStr.toStdString();
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

BigFixedPoint::BigFixedPoint(QString num)
{
    // Strip separators out
    for (int i = 0; i < num.size(); ++i)
    {
        if (num[i] == QLocale::system().groupSeparator())
        {
            num.remove(i, 1);
        }
    }

    // Where's the decimal point?
    int decimalLoc = num.indexOf(QLocale::system().decimalPoint());
    if (decimalLoc <= 0)
    {
        decimalPlaces = 0;
    } else {
        decimalPlaces = num.size()-1 - decimalLoc;
        num.remove(decimalLoc, 1);
    }

    // There must not be leading 0s or it will interpret the number as octal
    // That was puzzling for a few moments, let me tell you.
    int i = 0;
    assert(decimalPlaces >= 0);
    while ((i < (num.size() - decimalPlaces))
        && (num[i] == QLocale::system().zeroDigit()))
    {
        num.remove(i, 1);
        ++i;
    }

    // The number string is clean
    if (num.size() == 0)
    {
        number = 0;
    } else {
        number = num.toStdString();
    }
}

//! \todo Redundant
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

bool BigFixedPoint::isValid(QString numStr)
{
    // At start only: - (optional)
    // 0 or 1 decimal point, no more.
    // Sprinkle in group separators to your heart's content
    // Everything else is digits
    int pos = 0;
    bool foundNegative = false;
    QString gmp_str;

    if (numStr.isEmpty())
    {
        //qDebug() << "EMPTY";
        return false;
    }

    // Skip leading - and whitespace
    while ((pos < numStr.size())
        && (numStr[pos].isSpace()
            || numStr[pos] == QLocale::system().negativeSign()))
    {
        if (numStr[pos] == QLocale::system().negativeSign())
        {
            if (foundNegative)
            {
                //qDebug() << "Found extra negative: INVALID";
                return false;
            } else {
                foundNegative = true;
                gmp_str.append(numStr[pos]);
            }
        }
        ++pos;
    }

    bool foundDigit = false;
    // Now only digits, decimal and group separators
    while (pos < numStr.size())
    {
        if (!numStr[pos].isDigit()
            && numStr[pos] != QLocale::system().decimalPoint()
            && numStr[pos] != QLocale::system().groupSeparator())
        {
            return false;
        }

        if (numStr[pos].isDigit())
        {
            foundDigit = true;
            gmp_str.append(numStr[pos]);
        } else {
            // Not a digit
        }
        ++pos;
    }

    if (!foundDigit)
    {
        return false;
    }

    mpz_class num;
    if (num.set_str(gmp_str.toStdString(), 10) != 0)
    {
        return false;
    }

    // Made it this far, looks good
    return true;
}

void BigFixedPoint::scale(int decimals)
{
    int adjustment = decimals - decimalPlaces;
    if (adjustment > 0)
    {
        mpz_class factor;
        mpz_ui_pow_ui(factor.get_mpz_t(), 10, adjustment);
        mpz_class adjusted = number * factor;
        number = adjusted;
        decimalPlaces = decimals;
    } else if (adjustment < 0) {
        mpz_class factor;
        mpz_ui_pow_ui(factor.get_mpz_t(), 10, abs(adjustment));

        if (roundingEnabled)
        {
            //qDebug() << "Rounding " << QString::fromStdString(number.get_str())
            //        << "from " << decimalPlaces << " to " << decimals;
            mpz_class adjNum;
            mpz_ui_pow_ui(adjNum.get_mpz_t(), 10, abs(adjustment)-1);
            adjNum = 5*adjNum;
            //qDebug() << "Adjnum = " << QString::fromStdString(adjNum.get_str());
            number += adjNum;
            //qDebug() << "number adjusted = " << QString::fromStdString(number.get_str());
        }

        mpz_class adjusted = number / factor;
        number = adjusted;
        decimalPlaces = decimals;
    } else {
        // Scaled correctly, do nothing
    }
}

BigFixedPoint BigFixedPoint::pow(int power) const
{
    mpz_class res;
    mpz_pow_ui(res.get_mpz_t(), number.get_mpz_t(), power);
    return BigFixedPoint(res, decimalPlaces*power);
}

BigFixedPoint BigFixedPoint::root(int root) const
{
    mpz_class res;
    mpz_root(res.get_mpz_t(), number.get_mpz_t(), root);
    return BigFixedPoint(res, decimalPlaces/root);
}

BigFixedPoint BigFixedPoint::sqrt() const
{
    mpz_class res;
    mpz_sqrt(res.get_mpz_t(), number.get_mpz_t());
    return BigFixedPoint(res, decimalPlaces/2);
}

BigFixedPoint& BigFixedPoint::operator=(const BigFixedPoint &rhs)
{
    // Check for self-assignment
    if (this != &rhs)
    {
        number = rhs.number;
        decimalPlaces = rhs.decimalPlaces;
    }

    return *this;
}

BigFixedPoint& BigFixedPoint::operator=(int num)
{
    number = num;
    decimalPlaces = 0;
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

/*! Equal comparison operator.
 *
 * IMPORTANT: Technically, operators which are equal should behave exactly the
 * same. However, our scaling means two numbers with a different number
 * of decimal places will behave slightly differently, and yet be equal.
 * I prefer it this way, but it's good to take note of it.
 */
bool BigFixedPoint::operator==(int rhs) const {
    BigFixedPoint lhsScaled = *this;
    lhsScaled.scale(0);
    return (lhsScaled.number == rhs);
}

bool BigFixedPoint::operator!=(const BigFixedPoint &rhs) const {
  return !(*this == rhs);
}

bool BigFixedPoint::operator!=(int rhs) const {
    BigFixedPoint lhsScaled = *this;
    lhsScaled.scale(0);
    return (lhsScaled.number != rhs);
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


bool BigFixedPoint::operator>(qint64 rhs) const {
    BigFixedPoint lhsScaled = *this;
    lhsScaled.scale(0);
    BigFixedPoint rhsScaled(rhs);

    return lhsScaled > rhsScaled;
}

bool BigFixedPoint::operator<=(qint64 rhs) const {
    return !(*this > rhs);
}

bool BigFixedPoint::operator<(qint64 rhs) const {
    BigFixedPoint lhsScaled = *this;
    lhsScaled.scale(0);
    BigFixedPoint rhsScaled(rhs);

    return lhsScaled < rhsScaled;
}

bool BigFixedPoint::operator>=(qint64 rhs) const {
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

const BigFixedPoint operator+(const BigFixedPoint &lhs, const BigFixedPoint& rhs)
{
    return BigFixedPoint(lhs) += rhs;
}

BigFixedPoint& BigFixedPoint::operator+=(int rhs) {
    mpz_class factor;
    mpz_ui_pow_ui(factor.get_mpz_t(), 10, decimalPlaces);
    number = number + rhs*factor;
    return *this;
}

const BigFixedPoint operator+(const BigFixedPoint &lhs, int rhs)
{
    return BigFixedPoint(lhs) += rhs;
}

const BigFixedPoint operator+(int lhs, const BigFixedPoint &rhs)
{
    return BigFixedPoint(rhs) += lhs;
}

BigFixedPoint& BigFixedPoint::operator-=(const BigFixedPoint& rhs) {
    int decimals = decimalPlaces;
    // Scale parameter to our scale
    if (decimals > rhs.getDecimalPlaces())
    {
        int adjustment = decimals - rhs.getDecimalPlaces();
        mpz_class factor;
        mpz_ui_pow_ui(factor.get_mpz_t(), 10, adjustment);
        number = number - rhs.getValue()*factor;
    } else if (decimals < rhs.getDecimalPlaces()) {
        // Scale ourselves to the parameter
        decimals = rhs.getDecimalPlaces();
        int adjustment = rhs.decimalPlaces - decimalPlaces;
        mpz_class factor;
        mpz_ui_pow_ui(factor.get_mpz_t(), 10, adjustment);
        number = number*factor - rhs.getValue();
        decimalPlaces = decimals;
    } else {
        number -= rhs.getValue();
    }

    return *this;
}

const BigFixedPoint operator-(const BigFixedPoint& lhs, const BigFixedPoint& rhs)
{
    return BigFixedPoint(lhs) -= rhs;
}

BigFixedPoint& BigFixedPoint::operator-=(int rhs) {
    mpz_class factor;
    mpz_ui_pow_ui(factor.get_mpz_t(), 10, decimalPlaces);
    number = number - rhs*factor;
    return *this;
}

const BigFixedPoint operator-(const BigFixedPoint& lhs, int rhs)
{
    return BigFixedPoint(lhs) -= rhs;
}

const BigFixedPoint operator-(int lhs, const BigFixedPoint& rhs)
{
    return BigFixedPoint(lhs) -= rhs;
}

BigFixedPoint& BigFixedPoint::operator*=(const BigFixedPoint& rhs)
                         {
    int decimals = decimalPlaces + rhs.getDecimalPlaces();
    number = number*rhs.getValue();
    decimalPlaces = decimals;
    return *this;
}

const BigFixedPoint operator*(const BigFixedPoint& lhs,
                              const BigFixedPoint& rhs)
{
    return BigFixedPoint(lhs) *= rhs;
}

BigFixedPoint& BigFixedPoint::operator/=(const BigFixedPoint& rhs)
{
    assert(rhs.getDecimalPlaces() <= decimalPlaces);
    int decimals = decimalPlaces - rhs.getDecimalPlaces();
    number = number / rhs.getValue();
    decimalPlaces = decimals;
    return *this;
}

const BigFixedPoint operator/(const BigFixedPoint& lhs,
                              const BigFixedPoint& rhs)
{
    return BigFixedPoint(lhs) /= rhs;
}

BigFixedPoint& BigFixedPoint::operator/=(int rhs)
{
    number = number / rhs;
    return *this;
}

const BigFixedPoint operator/(BigFixedPoint& lhs, int rhs)
{
    return BigFixedPoint(lhs) /= rhs;
}

const BigFixedPoint operator%(const BigFixedPoint& lhs,
                              const BigFixedPoint& rhs)
{
    BigFixedPoint ret;

    assert(rhs.getDecimalPlaces() <= lhs.decimalPlaces);
    int decimals = lhs.decimalPlaces - rhs.getDecimalPlaces();
    ret.number = lhs.number % rhs.getValue();
    ret.decimalPlaces = decimals;
    return ret;
}

const BigFixedPoint operator%(BigFixedPoint& lhs, int rhs)
{
    BigFixedPoint ret;

    ret.number = lhs.number % rhs;
    ret.decimalPlaces = lhs.decimalPlaces;
    return ret;
}

/*! Gets a seed from an appropriate entropy source.
 */
quint32 BigFixedPoint::getSeed()
{
    quint32 seed;
#if defined(Q_OS_LINUX)
    std::ifstream urandom;
    urandom.open("/dev/urandom");
    urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));
    urandom.close();
#else // also available: Q_OS_WIN32 and Q_OS_MAC
    //seed = QDateTime::currentMSecsSinceEpoch();
    seed = QDateTime::toTime_t();
#endif
    return seed;
}

BigFixedPoint BigFixedPoint::random(const BigFixedPoint& min, const BigFixedPoint& max)
{
    BigFixedPoint range = max - min;
    range.decimalPlaces = std::max(min.decimalPlaces, max.decimalPlaces);
    return BigFixedPoint::random(range) + min;
}

BigFixedPoint BigFixedPoint::random(const BigFixedPoint& n)
{
    gmp_randclass r(gmp_randinit_default);

    //r.seed(QTime::currentTime().msec());
    r.seed(getSeed());
    mpz_class num = r.get_z_range(n.number+1);
    return BigFixedPoint(num, n.getDecimalPlaces());
}

BigFixedPoint BigFixedPoint::max(const BigFixedPoint& lhs, const BigFixedPoint& rhs)
{
    if (lhs > rhs)
        return lhs;
    else
        return rhs;
}

BigFixedPoint BigFixedPoint::min(const BigFixedPoint& lhs, const BigFixedPoint& rhs)
{
    if (lhs < rhs)
        return lhs;
    else
        return rhs;
}

QString BigFixedPoint::toString() const
{
    assert(decimalPlaces >= 0);

    QString str;
    QString numStr = QString::fromStdString(number.get_str());

    // Handle - sign
    bool negative = number < 0;
    if (negative)
    {
        int negPos = numStr.indexOf(QLocale::system().negativeSign());
        if (negPos >= 0)
        {
            numStr.remove(negPos, 1);
        }
    }

    // Add back leading 0's
    if (numStr.size() < (decimalPlaces+1))
    {
        int addNum = decimalPlaces + 1 - numStr.size();
        for (int i = 0; i < addNum; ++i)
        {
            numStr.insert(0, QLocale::system().zeroDigit());
        }
    }

    if (decimalPlaces == 0)
    {
        // Insert decimal separator into the integer part
        int partSize = numStr.size();
        for (int i = 1; i < partSize; ++i)
        {
            if ((i % 3) == 0)
            {
                int pos = (partSize) - i;
                numStr.insert(pos, QLocale::system().groupSeparator());
            }
        }

        if (negative)
        {
            numStr.prepend(QLocale::system().negativeSign());
        }

        str = numStr;
    } else {
        int decimalLoc = numStr.size() - decimalPlaces;
        QString integerPart = numStr.left(decimalLoc);
        QString fractionalPart = numStr.right(decimalPlaces);

        // Insert decimal separator into the integer part
        int partSize = integerPart.size();
        if (partSize > 4)
        {
            for (int i = 1; i < partSize; ++i)
            {
                if ((i % 3) == 0)
                {
                    int pos = partSize - i;
                    integerPart.insert(pos, QLocale::system().groupSeparator());
                }
            }
        }

        if (negative)
        {
            integerPart.prepend(QLocale::system().negativeSign());
        }

        // Insert decimal separator into the fractional part
        if (fractionalPart.size() > 4) {
            for (int i = 1; i < decimalPlaces; ++i)
            {
                int pos = i + (i/3)-1;
                if ((i % 3) == 0)
                {
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

qint64 BigFixedPoint::toLongLong() const
{
    if (fitsInLongLong())
    {
        return QString::fromStdString(number.get_str()).toLongLong();
    } else {
        if (*this < std::numeric_limits<qint64>::min())
        {
            return std::numeric_limits<qint64>::min();
        } else if (*this > std::numeric_limits<qint64>::max()) {
            return std::numeric_limits<qint64>::max();
        } else {
            return QString::fromStdString(number.get_str()).toLongLong();
        }
    }
}
