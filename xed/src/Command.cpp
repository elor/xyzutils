/*
 * Command.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: elor
 */

#include <Command.hpp>
#include <stdexcept>
#include <cstring>

using namespace std;

int Command::operator()(const Atom &original, Atom* out)
{
  bool selected = selector(original);

  switch (action)
  {
  case print:
    if (!selected)
    {
      return 1;
    }

    *out = original;
    return 0;
  case remove:
    if (selected)
    {
      return 1;
    }

    *out = original;
    return 0;
  default:
    throw runtime_error("unknown command during execution");
  }

  throw runtime_error(
      "reached invalid part of the code during command execution");
}

AtomSet Command::operator()(const AtomSet &in)
{
  AtomSet out;
  strcpy(out.comment, in.comment);

  Atom tmpAtom;

  for (AtomSet::Vector::const_iterator it = in.atoms.begin(); it
      != in.atoms.end(); ++it)
  {
    if ((*this)(*it, &tmpAtom) == 0)
    {
      out.atoms.push_back(tmpAtom);
    }
  }

  return out;
}
