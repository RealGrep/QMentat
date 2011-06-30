#ifndef RANDOM_H
#define RANDOM_H

#include <QtGlobal>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <fstream>   // Linux only - for reading urandom

template <class inttype> class RandomInt
{
public:
    RandomInt(quint64 seed, quint64 min, quint64 max)
        : engine(static_cast<quint64>(seed)),
        dist(min, max),
        gen(engine, dist) {}
    RandomInt(quint64 min, quint64 max)
        : engine(static_cast<quint64>(GetSeed())),
        dist(min, max),
        gen(engine, dist) {}

    inttype operator()(void) { return gen(); }
protected:
    boost::mt19937 engine;
    boost::uniform_int<inttype> dist;
    boost::variate_generator<boost::mt19937&, boost::uniform_int<quint64> > gen;

    static quint64 GetSeed()
    {
        quint64 seed;
        std::ifstream urandom;
        urandom.open("/dev/urandom");
        urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));
        urandom.close();
        return seed;
    }
};

/*! Gets a seed from an appropriate entropy source.
 * \todo Seeding from urandom only works in Linux. Work in an alternative for
 *    windows and other non-Unix systems.
 */
/*
static quint64 RandomInt::GetSeed()
{
   quint64 seed;
   std::ifstream urandom;
   urandom.open("/dev/urandom");
   urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));
   urandom.close();
   return seed;
}
*/

#endif // RANDOM_H
