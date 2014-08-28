/*
 * Bond.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: elor
 */

#include <Bond.hpp>

using namespace std;

Bond::Bond()
{
}

Bond::~Bond()
{
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
  file << distance << endl;
}
