/*
 * Bond.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#include "Bond.hpp"

using namespace std;

Bond::Bond()
{
  omp_init_lock(&lock);
}

Bond::~Bond()
{
  omp_destroy_lock(&lock);
  file.close();
}

void Bond::openFile(char *key)
{
  char filename[1024];
  sprintf(filename, "%s.txt", key);

  file.open(filename);
}

void Bond::write(double distance)
{
  omp_set_lock(&lock);

  file << distance << endl;

  omp_unset_lock(&lock);
}
