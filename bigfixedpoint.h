#ifndef BIGFIXEDPOINT_H
#define BIGFIXEDPOINT_H

#include <QString>
#include <gmpxx.h>
#include <string>
#include <iostream>
#include <cassert>

class BigFixedPoint {
public:
    BigFixedPoint();
    BigFixedPoint(int num);
    BigFixedPoint(QString num);
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

    void scale(int decimals);

    //static QString decimalize(mpz_class num, qint32 decimals);
    static BigFixedPoint random(const BigFixedPoint& min, const BigFixedPoint& max);
    static BigFixedPoint random(const BigFixedPoint& n);
    static BigFixedPoint max(const BigFixedPoint& lhs, const BigFixedPoint& rhs);
    static BigFixedPoint min(const BigFixedPoint& lhs, const BigFixedPoint& rhs);

    BigFixedPoint pow(int power) const;
    BigFixedPoint root(int root) const;
    BigFixedPoint sqrt() const;

    // Assignment operator
    BigFixedPoint& operator=(const BigFixedPoint &rhs);
    BigFixedPoint& operator=(int num);

    // Comparison operators
    bool operator==(const BigFixedPoint &rhs) const;
    bool operator==(int rhs) const;
    bool operator!=(const BigFixedPoint &rhs) const;
    bool operator!=(int rhs) const;
    bool operator>(const BigFixedPoint &rhs) const;
    bool operator<=(const BigFixedPoint &rhs) const;
    bool operator<(const BigFixedPoint &rhs) const;
    bool operator>=(const BigFixedPoint &rhs) const;

    // Operators for the basic 4 operations
    BigFixedPoint& operator+=(const BigFixedPoint& rhs);
    const BigFixedPoint operator+(const BigFixedPoint& rhs) const;
    BigFixedPoint& operator+=(int rhs);
    const BigFixedPoint operator+(int rhs) const;
    BigFixedPoint & operator-=(const BigFixedPoint& rhs);
    const BigFixedPoint operator-(const BigFixedPoint& rhs) const;
    BigFixedPoint& operator*=(const BigFixedPoint& rhs);
    const BigFixedPoint operator*(const BigFixedPoint& rhs) const;
    BigFixedPoint& operator/=(const BigFixedPoint& rhs);
    const BigFixedPoint operator/(const BigFixedPoint& rhs) const;
    BigFixedPoint& operator/=(int rhs);
    const BigFixedPoint operator/(int rhs) const;
    const BigFixedPoint operator%(const BigFixedPoint& rhs) const;
    const BigFixedPoint operator%(int rhs) const;

    QString toString() const;
    friend std::ostream&
            operator<< (std::ostream &os, const BigFixedPoint &x)
    {
        os << x.toString().toStdString();
        return os;
    }

private:
    mpz_class number;
    int decimalPlaces;
};

#endif // BIGFIXEDPOINT_H
