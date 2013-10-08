/*
 * CoordinationHistogrammData.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#include <CoordinationHistogramData.hpp>
#include <stdexcept>
#include <algorithm>

using namespace std;

CoordinationHistogramData::CoordinationHistogramData(size_t numBondTypes) :
  size(numBondTypes), size2(size * size)
{
  data = new Bins[size2];
}

CoordinationHistogramData::~CoordinationHistogramData()
{
  delete[] data;
}

size_t CoordinationHistogramData::getIndex(size_t type1, size_t type2)
{
  size_t index = type1 * size + type2;

  if (index > size2)
  {
    throw runtime_error("CoordinationHistogramData::getIndex: invalid index");
  }

  return index;
}

void CoordinationHistogramData::addValue(size_t type1, size_t type2,
    size_t numNeighbors)
{
  size_t index = getIndex(type1, type2);

  Bins &bins = data[index];
  if (bins.size() <= numNeighbors)
  {
    bins.resize(numNeighbors + 1, 0);
  }

  bins[numNeighbors] += 1;
}
