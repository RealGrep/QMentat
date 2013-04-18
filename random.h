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
#ifndef RANDOM_H
#define RANDOM_H

#include <QtGlobal>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#if defined(Q_OS_LINUX)
#   include <fstream>   // Linux only - for reading urandom
#else
#   include <QDateTime>
#endif

template <class inttype> class RandomInt
{
public:
    RandomInt(quint64 seed, quint64 min, quint64 max)
        : engine(static_cast<quint64>(seed)),
        dist(min, max),
        gen(engine, dist) {}
    RandomInt(quint64 min, quint64 max)
        : engine(static_cast<quint64>(getSeed())),
        dist(min, max),
        gen(engine, dist) {}

    inttype operator()(void) { return gen(); }
protected:
    boost::mt19937 engine;
    boost::uniform_int<inttype> dist;
    boost::variate_generator<boost::mt19937&, boost::uniform_int<inttype> > gen;

    /*! Gets a seed from an appropriate entropy source.
     */
    static quint64 getSeed()
    {
        quint64 seed;
#if defined(Q_OS_LINUX)
        std::ifstream urandom;
        urandom.open("/dev/urandom");
        urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));
        urandom.close();
#else // also available: Q_OS_WIN32 and Q_OS_MAC
        seed = QDateTime::currentMSecsSinceEpoch();
#endif
        return seed;
    }
};

#endif // RANDOM_H
