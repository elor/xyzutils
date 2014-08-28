/*
 * Type.h
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#ifndef TYPE_H_
#define TYPE_H_

#include <string>

class Type
{
public:
  std::string name;
  size_t id;
  size_t count;

  Type(const char *_name, size_t _id);
  ~Type();
};

#endif /* TYPE_H_ */
