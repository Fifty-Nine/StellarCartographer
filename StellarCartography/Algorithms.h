#ifndef SC_ALGORITHMS_H
#define SC_ALGORITHMS_H

#include <list>

#include "StellarCartography/Coordinate.h"

namespace StellarCartography
{

namespace Detail
{

typedef std::pair<Coordinate, double> Sample;
typedef std::list<Sample> SampleList;
typedef std::pair<Coordinate, Coordinate> Solution;

Solution trilaterateOne(
    const Sample& s1, const Sample& s2, const Sample& s3);

Coordinate trilaterateMany(const SampleList& samples);

}

/*
 * Trilaterate a range of (Coordinate, Distance) points, returning
 * a best guess for the unknown position. Given only three points, 
 * two solutions are possible and one is discarded arbitrarily. Given
 * four or more points, only one solution is possible (if any). In the
 * case where none of the solutions matches, an exception is thrown.
 */
template<class It>
Coordinate trilaterate(It begin, It end)
{
    return Detail::trilaterateMany(Detail::SampleList(begin, end));
}

}

#endif /* SC_ALGORITHMS_H */
