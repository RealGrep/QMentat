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
#ifndef RANDOM_H
#define RANDOM_H

#include <QtGlobal>
#include <random>
#include <QRandomGenerator>

template <class inttype> class RandomInt
{
public:
    RandomInt(quint64 seed, quint64 min, quint64 max)
        : engine(static_cast<quint64>(seed)),
        dist(min, max) {}
    RandomInt(quint64 min, quint64 max)
        : engine(static_cast<quint64>(getSeed())),
        dist(min, max) {}

    inttype operator()(void) { return dist(engine); }

protected:
    std::mt19937_64 engine;
    std::uniform_int_distribution<inttype> dist;

    /*! Gets a seed from an appropriate entropy source.
     */
    static quint64 getSeed()
    {
        quint32 seed = QRandomGenerator::securelySeeded().generate();

        return seed;
    }
};

#endif // RANDOM_H
