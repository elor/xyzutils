/*
 * Atom.h
 *
 *  Created on: Sep 24, 2013
 *      Author: elor
 */

#ifndef ATOM_H_
#define ATOM_H_

#include <ostream>

class Atom
{
public:
  double pos[3];
  char type[32];

  void parse(const char *source);
};

std::ostream &operator<<(std::ostream &os, const Atom &atom);

#endif /* ATOM_H_ */
