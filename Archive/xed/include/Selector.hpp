/*
 * Selector.h
 *
 *  Created on: Sep 24, 2013
 *      Author: elor
 */

#ifndef SELECTOR_H_
#define SELECTOR_H_

class Atom;

class Selector
{
public:
  enum
  {
    x = 0, // x/,/
    y = 1, // y/,/
    z = 2
  // z/,/
  } axis;

  double min;
  double max;

  static const double any;

  bool operator()(const Atom &atom);
};

#endif /* SELECTOR_H_ */
