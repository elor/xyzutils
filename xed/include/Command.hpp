/*
 * Command.h
 *
 *  Created on: Sep 24, 2013
 *      Author: elor
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <AtomSet.hpp>
#include <Selector.hpp>

class Command
{
public:
  Selector selector;

  enum
  {
    print, // prints if selected, removes otherwise, 'p'
    remove
  // removes if selected, prints otherwise, 'd'
  } action;

  int operator()(const Atom &original, Atom* out);
  AtomSet operator()(const AtomSet &in);
};

#endif /* COMMAND_H_ */
