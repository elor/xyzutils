/*
 * BondMap.h
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#ifndef BONDMAP_H_
#define BONDMAP_H_

#include <TypeVec.hpp>
#include <Bond.hpp>

class BondMap
{
private:
  Bond *bonds;
  size_t size;

  size_t getFileIndex(size_t mintype, size_t maxtype);
  void makeKey(const char *t1, const char *t2, char *out_key);

public:
  BondMap(const TypeVec &types);
  virtual ~BondMap();

  Bond *getFile(size_t type1, size_t type2);
};

#endif /* BONDMAP_H_ */
