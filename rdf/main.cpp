#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>

using namespace std;

struct stuff
{
  double V;
  int a_num;
  int b_num;
  double a_rho; // number density
  double b_rho; // number density
  double delta_r; // thickness of shell
};

stuff readOutTypesTxt(char *filename, char *first, char *second)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("can't open out_types file for reading");
  }

  double dimensions[6];
  file >> dimensions[0] >> dimensions[1] >> dimensions[2] >> dimensions[3] >> dimensions[4] >> dimensions[5]; 

  double span[3] = {
    dimensions[1] - dimensions[0],
    dimensions[3] - dimensions[2],
    dimensions[5] - dimensions[4]
  };

  stuff some;
  some.V = span[0] * span[1] * span[2];
  some.delta_r = 0.02;  // Angstrom
  some.a_num = some.b_num = 0;

  char type[64];
  int atomcount;

  for (; !file.eof(); )
  {
    file >> type >> atomcount;
    if (!strcmp(type, first))
    {
      some.a_num = atomcount;
      some.a_rho = double(atomcount) / some.V;
    }
    if (!strcmp(type, second))
    {
      some.b_num = atomcount;
      some.b_rho = double(atomcount) / some.V;
    }

    if (some.a_num != 0 && some.b_num != 0)
    {
      break;
    }
  }
  
  if (some.a_num == 0 || some.b_num == 0)
  {
    throw runtime_error("some number not found");
  }

  file.close();

  return some;
}

void printRDF(char *filename, stuff some)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("can't open distance data file for reading");
  }

  double r = 0.0;
  double current;
  double rdf = 0.0;

  file >> current;

  for (; !file.eof() ;)
  {
    cout << r << "\t" << rdf << endl;
    rdf = 0.0;
    r += some.delta_r;

    while (current < r)
    {
      rdf += 1.0;
      file >> current;
      if (file.eof())
      {
        break;
      }
    }

    rdf /= 4 * M_PI * some.b_rho * r*r / some.delta_r;
  }

  cout << r << "\t" << rdf << endl;
}

int main(int argc, char **argv)
{
  if (argc != 5)
  {
    cout << "Syntax: " << argv[0] << " <data_file.txt> <out_types.txt> <first atom type> <second atom type>" << endl;
    return 1;
  }

  stuff some = readOutTypesTxt(argv[2], argv[3], argv[4]);

  printRDF(argv[1], some);


  return 0;
}

