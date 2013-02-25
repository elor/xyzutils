#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <omp.h>
#include <string>

using namespace std;

struct bin_t
{
  size_t count;
  double zmax;

  bin_t() : count(0), zmax(0.0) {}
};

typedef map<size_t, bin_t> binvec;
typedef map<size_t, binvec> binfield;

int readFile(const char *filename, binfield &out_binfield, const double binwidth)
{
  ifstream file(filename);

  if (!file.is_open())
  {
    throw runtime_error("can't open input file for reading");
  }

  int numatoms;
  file >> numatoms;

  char type[1024];

  file.getline(type, 1024);
  file.getline(type, 1024);

  double x, y, z;
  size_t xbin, ybin;
  bin_t *bin;

  for (int i=0; i < numatoms; ++i)
  {
    file >> type;
    file >> x;
    file >> y;
    file >> z;

    xbin = size_t(x/binwidth);
    ybin = size_t(y/binwidth);

    bin = &out_binfield[xbin][ybin];

    bin->count++;
    if (z > bin->zmax)
    {
      bin->zmax = z;
    }

    if (file.eof())
    {
      throw runtime_error("end of xyz file reached prematurely");
    }

    // discard possibly remaining values (vel, ...)
    file.getline(type, 1023);
  }

  return numatoms;
}

void printBinField(binfield &bins, size_t numatoms, double binwidth)
{
  double normfactor = 1. / numatoms;
  int xbin, ybin;
  xbin = 0;
  for (binfield::iterator it = bins.begin(); it != bins.end(); ++it, ++xbin)
  {
    ybin = 0;
    for (binvec::iterator ti = it->second.begin(); ti != it->second.end(); ++ti, ++ybin)
    {
      cout << xbin*binwidth << "\t" << ybin*binwidth << "\t" << ti->second.zmax << "\t" << ti->second.count * normfactor << endl;
    }

    cout << endl;
  }
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    cout << "Syntax: " << argv[0] << " <filename.xyz> <bin width>" << endl;
    return 1;
  }

  binfield bins;

  double binwidth = atof(argv[2]);

  // read atoms
  size_t numatoms = readFile(argv[1], bins, binwidth);

  // output bin field
  printBinField(bins, numatoms, binwidth);

  return 0;
}

