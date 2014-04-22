/*
 * Type.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#include <Type.hpp>

Type::Type(const char *_name, size_t _id) :
  name(_name), id(_id), count(0)
{
}

Type::~Type()
{
}
