/*
 * Atom.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: elor
 */

#include <Atom.hpp>
#include <cstdio>
#include <iostream>
#include <stdexcept>

using namespace std;

void Atom::parse(const char *source)
{
  if (sscanf(source, "%s %lf %lf %lf", type, &pos[0], &pos[1], &pos[2]) != 4)
  {
    cerr << "error while parsing atom line: " << source << endl;
    throw runtime_error("error while parsing atom line");
  }
}

std::ostream &operator<<(std::ostream &os, const Atom &atom)
{
  os << atom.type << " " << atom.pos[0] << " " << atom.pos[1] << " "
      << atom.pos[2];
  return os;
}
