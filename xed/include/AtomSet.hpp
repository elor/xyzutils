/*
 * AtomSet.h
 *
 *  Created on: Sep 24, 2013
 *      Author: elor
 */

#ifndef ATOMSET_H_
#define ATOMSET_H_

#include <Atom.hpp>
#include <iostream>
#include <vector>

class AtomSet
{
public:
  typedef std::vector<Atom> Vector;
  Vector atoms;
  char comment[1024];

  static AtomSet create(std::istream &in);
};

std::ostream &operator<<(std::ostream &os, const AtomSet &atomSet);
std::ostream &operator<<(std::ostream &os, const AtomSet *atomSet);

#endif /* ATOMSET_H_ */
