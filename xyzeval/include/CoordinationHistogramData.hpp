/*
 * CoordinationHistogramData.h
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#ifndef COORDINATIONHISTOGRAMDATA_H_
#define COORDINATIONHISTOGRAMDATA_H_

#include <vector>
#include <cstdlib>

class CoordinationHistogramData
{
public:
  typedef std::vector<size_t> Bins;

  Bins *data;
  size_t size, size2;

  CoordinationHistogramData(size_t numBondTypes);
  virtual ~CoordinationHistogramData();

  size_t getIndex(size_t type1, size_t type2);
  void addValue(size_t type1, size_t type2, size_t numNeighbors);
};

#endif /* COORDINATIONHISTOGRAMDATA_H_ */
