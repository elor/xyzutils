/*
 * Bond.h
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#ifndef BOND_H_
#define BOND_H_

#include <fstream>

class Bond
{
private:
  std::ofstream file;

public:
  Bond();
  ~Bond();

  void openFile(char *key);
  void write(double distance);
};

#endif /* BOND_H_ */
