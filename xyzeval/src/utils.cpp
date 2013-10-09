/*
 * utils.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: elor
 */

#include <utils.hpp>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

vector<string> strsplit(string str)
{
  istringstream iss(str);
  vector<string> words;

  copy(istream_iterator<string> (iss), istream_iterator<string> (),
      back_insert_iterator<vector<string> > (words));

  return words;
}

vector<string> strsplit(const char *str)
{
  return strsplit(string(str));
}
