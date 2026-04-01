/* Copyright (c) 2026 Mike Dusseault
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
#ifndef BIGFIXEDPOINT_H
#define BIGFIXEDPOINT_H

#include <QString>
#include <gmpxx.h>
#include <string>
#include <iostream>
#include <cassert>
#include <limits>

#include <QDebug>

class BigFixedPoint {
public:
    BigFixedPoint(const BigFixedPoint& bfp) = default;
    BigFixedPoint(BigFixedPoint&& other) noexcept;
    BigFixedPoint();
    BigFixedPoint(int num);
    BigFixedPoint(qint64 num);
    BigFixedPoint(QString num);
    BigFixedPoint(std::string num);
    BigFixedPoint(std::string num, int decimals);
    BigFixedPoint(mpz_class num, int decimals);

    ~BigFixedPoint() = default;

    [[nodiscard]] static bool isValid(QString numStr);

    [[nodiscard]] mpz_class getValue() const
    {
        return number;
    }

    void setValue(const mpz_class num)
    {
        number = num;
    }

    [[nodiscard]] int getDecimalPlaces() const noexcept
    {
        return decimalPlaces;
    }

    void setDecimalPlaces(int decimals)
    {
        decimalPlaces = decimals;
    }

    static void setRounding(bool rnd)
    {
        roundingEnabled = rnd;
    }

    [[nodiscard]] static bool isRounding() noexcept
    {
        return roundingEnabled;
    }

    [[nodiscard]] bool fitsInLongLong() const
    {
        if (decimalPlaces != 0)
        {
            return false;
        }

        if (*this < std::numeric_limits<qint64>::min())
        {
            return false;
        } else if (*this > std::numeric_limits<qint64>::max()) {
            return false;
        }

        QString numStr = QString::fromStdString(number.get_str());
        bool ok;
        numStr.toLongLong(&ok);
        if (!ok)
        {
            return false;
        }

        return true;
    }

    void scale(int decimals);

    static quint32 getSeed();
    [[nodiscard]] static BigFixedPoint random(const BigFixedPoint& min, const BigFixedPoint& max);
    [[nodiscard]] static BigFixedPoint random(const BigFixedPoint& n);
    [[nodiscard]] static BigFixedPoint max(const BigFixedPoint& lhs, const BigFixedPoint& rhs);
    [[nodiscard]] static BigFixedPoint min(const BigFixedPoint& lhs, const BigFixedPoint& rhs);

    [[nodiscard]] BigFixedPoint pow(int power) const;
    [[nodiscard]] BigFixedPoint root(int root) const;
    [[nodiscard]] BigFixedPoint sqrt() const;

    // Assignment operator
    BigFixedPoint& operator=(const BigFixedPoint &rhs);
    BigFixedPoint& operator=(BigFixedPoint&& rhs) noexcept;
    BigFixedPoint& operator=(int num);

    // Comparison operators
    [[nodiscard]] bool operator==(const BigFixedPoint &rhs) const;
    [[nodiscard]] bool operator==(int rhs) const;
    [[nodiscard]] bool operator!=(const BigFixedPoint &rhs) const;
    [[nodiscard]] bool operator!=(int rhs) const;
    [[nodiscard]] bool operator>(const BigFixedPoint &rhs) const;
    [[nodiscard]] bool operator<=(const BigFixedPoint &rhs) const;
    [[nodiscard]] bool operator<(const BigFixedPoint &rhs) const;
    [[nodiscard]] bool operator>=(const BigFixedPoint &rhs) const;

    [[nodiscard]] bool operator>(qint64 rhs) const;
    [[nodiscard]] bool operator<=(qint64 rhs) const;
    [[nodiscard]] bool operator<(qint64 rhs) const;
    [[nodiscard]] bool operator>=(qint64 rhs) const;

    // Operators for the basic 4 operations
    BigFixedPoint& operator+=(const BigFixedPoint& rhs);
    friend BigFixedPoint operator+(const BigFixedPoint &lhs, const BigFixedPoint& rhs);
    BigFixedPoint& operator+=(int rhs);
    friend BigFixedPoint operator+(const BigFixedPoint &lhs, int rhs);
    friend BigFixedPoint operator+(int lhs, const BigFixedPoint &rhs);

    BigFixedPoint & operator-=(const BigFixedPoint& rhs);
    friend BigFixedPoint operator-(const BigFixedPoint& lhs, const BigFixedPoint& rhs);
    BigFixedPoint& operator-=(int rhs);
    friend BigFixedPoint operator-(const BigFixedPoint &lhs, int rhs);
    friend BigFixedPoint operator-(int lhs, const BigFixedPoint &rhs);

    BigFixedPoint& operator*=(const BigFixedPoint& rhs);
    friend BigFixedPoint operator*(const BigFixedPoint& lhs,
                                         const BigFixedPoint& rhs);

    BigFixedPoint& operator/=(const BigFixedPoint& rhs);
    friend BigFixedPoint operator/(const BigFixedPoint& lhs,
                                         const BigFixedPoint& rhs);
    BigFixedPoint& operator/=(int rhs);
    friend BigFixedPoint operator/(BigFixedPoint& lhs, int rhs);
    friend BigFixedPoint operator%(const BigFixedPoint& lhs,
                                         const BigFixedPoint& rhs);
    friend BigFixedPoint operator%(BigFixedPoint& lhs, int rhs);

    [[nodiscard]] QString toString() const;
    [[nodiscard]] qint64 toLongLong() const;

    friend std::ostream&
            operator<< (std::ostream &os, const BigFixedPoint &x)
    {
        os << x.toString().toStdString();
        return os;
    }

private:
    mpz_class number;
    int decimalPlaces;
    static bool roundingEnabled;
    static bool isSeeded;
    static gmp_randclass r;
};

#endif // BIGFIXEDPOINT_H
