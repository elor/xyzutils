/*
 * AtomSet.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: elor
 */

#include <AtomSet.hpp>
#include <stdexcept>
#include <convert.hpp>

using namespace std;

AtomSet AtomSet::create(istream &in)
{
  if (in.eof())
  {
    throw runtime_error("can't read from input stream");
  }

  AtomSet atomSet;

  char buffer[1024];
  in.getline(buffer, 1024);
  unsigned int numatoms = 0;

  if (convert(buffer, &numatoms))
  {
    cerr << "corrupt atom count line: " << buffer << endl;
    throw runtime_error("corrupt atom count line");
  }

  if (in.eof())
  {
    throw runtime_error("unexpected end of file");
  }

  in.getline(atomSet.comment, 1024);

  Atom tmpAtom;

  for (size_t i = 0; i < numatoms; ++i)
  {
    if (in.eof())
    {
      throw runtime_error("unexpected end of file");
    }

    in.getline(buffer, 1024);

    tmpAtom.parse(buffer);

    atomSet.atoms.push_back(tmpAtom);
  }

  return atomSet;
}

std::ostream &operator<<(std::ostream &os, const AtomSet &atomSet)
{
  os << atomSet.atoms.size() << endl;
  os << atomSet.comment << endl;

  for (AtomSet::Vector::const_iterator it = atomSet.atoms.begin(); it
      != atomSet.atoms.end(); ++it)
  {
    os << *it << endl;
  }

  return os;
}

std::ostream &operator<<(std::ostream &os, const AtomSet *atomSet)
{
  os << *atomSet;
  return os;
}
