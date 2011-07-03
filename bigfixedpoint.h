#ifndef BIGFIXEDPOINT_H
#define BIGFIXEDPOINT_H

#include <QString>
#include <gmpxx.h>
#include <string>
#include <iostream>
#include <cassert>

class BigFixedPoint {
public:
    BigFixedPoint(std::string num);
    BigFixedPoint(std::string num, int decimals);
    BigFixedPoint(mpz_class num, int decimals);

    virtual ~BigFixedPoint();

    BigFixedPoint(const BigFixedPoint& bfp);

    mpz_class getValue() const
    {
        return number;
    }

    int getDecimalPlaces() const
    {
        return decimalPlaces;
    }

    void test();
    void scale(int decimals);

    static QString decimalize(mpz_class num, qint32 decimals);
    static BigFixedPoint random(const BigFixedPoint& min, const BigFixedPoint& max);
    static BigFixedPoint random(const BigFixedPoint& n);

    // Assignment operator
    BigFixedPoint& operator=(const BigFixedPoint &rhs);

    // Comparison operators
    bool operator==(const BigFixedPoint &rhs) const;
    bool operator!=(const BigFixedPoint &rhs) const;
    bool operator>(const BigFixedPoint &rhs) const;
    bool operator<=(const BigFixedPoint &rhs) const;
    bool operator<(const BigFixedPoint &rhs) const;
    bool operator>=(const BigFixedPoint &rhs) const;

    // Operators for the basic 4 operations
    BigFixedPoint& operator+=(const BigFixedPoint& y);
    const BigFixedPoint operator+(const BigFixedPoint& y) const;
    BigFixedPoint & operator-=(const BigFixedPoint& y);
    const BigFixedPoint operator-(const BigFixedPoint& y) const;
    BigFixedPoint& operator*=(const BigFixedPoint& y);
    const BigFixedPoint operator*(const BigFixedPoint& y) const;
    BigFixedPoint& operator/=(const BigFixedPoint& y);
    const BigFixedPoint operator/(const BigFixedPoint& y) const;

    friend std::ostream&
            operator<< (std::ostream &os, const BigFixedPoint &x)
    {
        os << decimalize(x.number, x.decimalPlaces).toStdString();
        return os;
    }

private:
    mpz_class number;
    int decimalPlaces;
};

#endif // BIGFIXEDPOINT_H
