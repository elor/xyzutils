/*
 * Selector.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: elor
 */

#include <Selector.hpp>
#include <Atom.hpp>

unsigned long long deadbeef = 0xdeadbeef;
const double Selector::any = *reinterpret_cast<const double*> (&deadbeef);

bool Selector::operator()(const Atom &atom)
{
  return (min == any || atom.pos[axis] >= min) && (max == any || atom.pos[axis]
      <= max);
}
