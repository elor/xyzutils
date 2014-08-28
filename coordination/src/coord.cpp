#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <stdexcept>
#include <cmath>

using namespace std;

struct atom_t {
  char *type;
  int typenum;
  double pos[3];
};

typedef map<char*, int> typemap;

int readFile(char *filename, typemap *out_types, atom_t **out_atoms)
{
  ifstream file(filename);

  if (!file.is_open())
  {
    throw runtime_error("can't open input file for reading");
  }

  int numatoms;
  file >> numatoms;
  file.ignore(256, '\n');
  file.ignore(256, '\n');

  atom_t *atoms = new atom_t[numatoms];
  char type[64];

  double min[3], max[3];
  bool unset=true;

  for (int i=0; i < numatoms; ++i)
  {
    file >> type;
    file >> atoms[i].pos[0];
    file >> atoms[i].pos[1];
    file >> atoms[i].pos[2];

    if (unset)
    {
      for (int k = 0; k < 3; ++k)
      {
        min[k] = max[k] = atoms[i].pos[k];
      }
      unset = false;
    }
    else
    {
      for (int k = 0; k < 3; ++k)
      {
        if (min[k] > atoms[i].pos[k])
        {
          min[k] = atoms[i].pos[k];
        }
        else if (max[k] < atoms[i].pos[k])
        {
          max[k] = atoms[i].pos[k];
        }
      }
    }

    int typenum = 0;
    for (typemap::iterator it = out_types->begin(); ; ++it, ++typenum)
    {
      if (it == out_types->end())
      {
        char *tmptype = new char[64];
        strcpy(tmptype, type);
        out_types->insert(typemap::value_type(tmptype, 1));
        atoms[i].type = tmptype;
        atoms[i].typenum = typenum;
        break;
      }
      else if (!strcmp(type, it->first))
      {
        atoms[i].type = it->first;
        atoms[i].typenum = typenum;
        ++it->second;
        break;
      }
    }

    if (file.eof())
    {
      break;
    }
  }

  *out_atoms = atoms;

  return numatoms;
}

double getDistance(const atom_t &a, const atom_t &b)
{
  double sum = 0.0;
  double tmp;

  for (int i = 0; i < 3; ++i)
  {
    tmp = a.pos[i] - b.pos[i];
    sum += tmp*tmp;
  }

  return sqrt(sum);
}

void printCoordinations(atom_t *atoms, int numatoms)
{
  // iterate from numatoms to 1 
  for (int i = numatoms; i--; )
  {
    int coords[5] = {0, 0, 0, 0, 0}; // 5 is a sufficiently high number for most systems
    // iterate from j-1 to 0
    for (int j = numatoms; j--; )
    {
      if (j == i)
      {
        continue;
      }

      double rmax = 0.0;

      switch (atoms[i].typenum)
      {
      case 0:
        switch (atoms[j].typenum)
        {
        case 0:
          rmax = 3.7;
          break;
        case 1:
          rmax = 2.2;
          break;
        default:
          cerr << j << endl;
          throw runtime_error("invalid type (j switch 0)");
        }
        break;
      case 1:
        switch (atoms[j].typenum)
        {
        case 0:
          rmax = 2.2;
          break;
        case 1:
          rmax = 3.2;
          break;
        default:
          cerr << j << endl;
          throw runtime_error("invalid type (j switch 1)");
        }
        break;
      default:
          cerr << i << endl;
          throw runtime_error("invalid type (i switch)");
      }

      if (getDistance(atoms[i], atoms[j]) < rmax)
      {
        coords[atoms[j].typenum]++;
      }
    }

    for (int j = 0; j < 5; ++j)
    {
      if (coords[j])
      {
        cout << atoms[i].typenum << "-" << j << "\t" << coords[j] << endl;
      }
    }
  }
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    cout << "Syntax: " << argv[0] << " <filename.xyz>" << endl;
    return 1;
  }
  typemap types;
  atom_t *atoms;

//  cout << "reading atoms from file " << argv[1] << endl;

  int numatoms = readFile(argv[1], &types, &atoms);

//  cout << numatoms << " atoms read" << endl;

  printCoordinations(atoms, numatoms);

  return 0;
}

