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
