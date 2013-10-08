/*
 * Bond.h
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#ifndef BOND_H_
#define BOND_H_

#include <fstream>
#include <omp.h>

class Bond
{
private:
  std::ofstream file;
  omp_lock_t lock;

public:
  Bond();
  ~Bond();

  void openFile(char *key);
  void write(double distance);
};

#endif /* BOND_H_ */
