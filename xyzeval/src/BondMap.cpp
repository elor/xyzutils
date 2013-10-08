/*
 * BondMap.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#include <BondMap.hpp>
#include <algorithm>
#include <stdexcept>

using namespace std;

BondMap::BondMap(const TypeVec &types)
{
  char key[1024];

  size = types.size();
  size = size * (size + 1) / 2;

  bonds = new Bond[size];

  size_t i = 0;

  for (TypeVec::const_iterator it = types.begin(); it != types.end(); ++it)
  {

    for (TypeVec::const_iterator ti = it; ti != types.end(); ++ti)
    {
      makeKey(it->name.c_str(), ti->name.c_str(), key);
      //      cout << "key: " << key << endl;

      bonds[i].openFile(key);
      ++i;
    }
  }
}

BondMap::~BondMap()
{
  delete[] bonds;
}

size_t BondMap::getFileIndex(size_t mintype, size_t maxtype)
{
  if (mintype > maxtype)
  {
    swap(mintype, maxtype);
  }

  size_t index = maxtype * (maxtype + 1) / 2 + mintype;

  if (index > size)
  {
    throw runtime_error("BondMap::getFileIndex: invalid index");
  }

  return index;
}

Bond *BondMap::getFile(size_t type1, size_t type2)
{
  return &bonds[getFileIndex(type1, type2)];
}

void BondMap::makeKey(const char *t1, const char *t2, char *out_key)
{
  sprintf(out_key, "%s-%s", t1, t2);
}
