/*
 * SquareSelector.h
 *
 *  Created on: Oct 9, 2013
 *      Author: elor
 */

#ifndef SQUARESELECTOR_H_
#define SQUARESELECTOR_H_

#include <vector>
#include <cmath>
#include <stdexcept>

template<class Type, class Container = std::vector<Type> >
class SquareSelector
{
private:
  Container data;
  size_t size;
public:
  SquareSelector(Container _data) :
    data(_data), size(sqrt(data.size()))
  {
    if (size * size != data.size())
    {
      throw std::runtime_error("SquareSelector: container has no square size");
    }
  }

  virtual ~SquareSelector()
  {
  }

  Type select(size_t i, size_t j)
  {
    size_t index = i * size + j;
    if (index >= data.size())
    {
      throw std::runtime_error("SquareSelector: index out of range");
    }

    return data[index];
  }

  const Type select(size_t i, size_t j) const
  {
    size_t index = i * size + j;
    if (index >= data.size())
    {
      throw std::runtime_error("SquareSelector: index out of range");
    }

    return data[index];
  }

  bool empty() const
  {
    return size == 0;
  }
};

#endif /* SQUARESELECTOR_H_ */
