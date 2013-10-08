#include <excr.hpp>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <stdexcept>

#include <iostream>

using namespace std;

excr::excr(char *input)
{
  createMap(input);
  smap::iterator it = props.find("Cycle");

  if (it == props.end())
  {
    throw runtime_error("Cycle data not found");
  }

  errno = 0;
  char *p;
  const char *str = it->second.c_str();
  cycle = strtol(str, &p, 10);

  if (errno != 0 || str == p)
  {
    throw runtime_error("excr: cycle number is no unsigned integer");
  }

  it = props.find("Comment");
  if (it == props.end())
  {
    throw runtime_error("Comment data not found");
  }
  comment = new char[it->second.size() + 1];
  strcpy(comment, it->second.c_str());

  it = props.find("Size");
  if (it == props.end())
  {
    // fallback to default exyz format
    it = props.find("Lattice");
    if (it == props.end())
    {
      throw runtime_error(
          "neither Size nor Lattice data found in xyz comment line");
    }
    vector<double> vec = split(it->second.c_str());
    xsize = vec[0];
    ysize = vec[4];
    height = vec[8];
  }
  else
  {
    vector<double> vec = split(it->second.c_str());
    xsize = vec[0];
    ysize = vec[1];
    height = vec[2];
  }
}

excr::~excr()
{
  if (comment)
  {
    delete[] comment;
    comment = NULL;
  }
}

void excr::createMap(char *input)
{
  char *prop = strtok(input, "=");

  while (prop != NULL)
  {
    if (prop[0] == ' ' || prop[0] == '\t')
    {
      prop += 1;
    }

    char *tok = strtok(NULL, " \t");
    if (tok == NULL)
    {
      throw runtime_error("malformed exyz properties");
    }

    if (tok[0] == '"')
    {
      if (tok[strlen(tok) - 1] != '"')
      {
        props[prop] += tok + 1;
        tok = strtok(NULL, "\"");
        props[prop] += " ";
        props[prop] += tok;
      }
      else
      {
        tok[strlen(tok) - 1] = '\0';
        props[prop] += tok + 1;
      }
    }
    else
    {
      props[prop] += tok;
    }

    prop = strtok(NULL, "=");
  }
}

vector<double> excr::split(const char *str)
{
  stringstream ss(stringstream::in | stringstream::out);
  vector<double> outvec;
  ss << str;

  double val;
  while (!ss.eof())
  {
    ss >> val;
    outvec.push_back(val);
  }

  return outvec;
}
