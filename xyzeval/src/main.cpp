#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <omp.h>

#include <Atom.hpp>
#include <BondMap.hpp>
#include <TypeVec.hpp>
#include <ArgumentParser.h>

using namespace std;

ofstream out("out_types.txt");

int readFile(const char *filename, TypeVec *out_types, Atom **out_atoms)
{
  ifstream file(filename);

  if (!file.is_open())
  {
    cerr << "can't open '" << filename << "' for reading" << endl;
    exit(1);
  }

  int numatoms;
  file >> numatoms;

  Atom *atoms = new Atom[numatoms];
  char type[1024];

  char comment[1024];
  file.getline(comment, 1024); // discard \n
  file.getline(comment, 1024); // read comment

  double min[3] =
  { 0.0, 0.0, 0.0 };
  double max[3] =
  { 0.0, 0.0, 0.0 };
  bool unset = true;

  for (int i = 0; i < numatoms; ++i)
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

    for (TypeVec::iterator it = out_types->begin();; ++it)
    {
      if (it == out_types->end())
      {
        out_types->push_back(Type(type, out_types->size()));
        break;
      }
      else if (it->name.compare(type) == 0)
      {
        atoms[i].type = it->id;
        ++(it->count);
        break;
      }
    }

    if (file.eof())
    {
      cerr << "unexpected end of file '" << filename << "'" << endl;
      exit(1);
    }
  }

  //  cerr << "min[0] max[0] min[1] max[1] min[2] max[2]" << endl;
  out << min[0] << " " << max[0] << "\t" << min[1] << " " << max[1] << "\t"
      << min[2] << " " << max[2] << endl;

  *out_atoms = atoms;

  return numatoms;
}

double getDistance(const Atom &a, const Atom &b)
{
  double sum = 0.0;
  double tmp;

  for (int i = 0; i < 3; ++i)
  {
    tmp = a.pos[i] - b.pos[i];
    sum += tmp * tmp;
  }

  return sqrt(sum);
}

void printInteratomicDistances(Atom *atoms, int numatoms, const double cutoff,
    BondMap *bonds)
{
  // iterate from numatoms to 1
  Bond *file;
#pragma omp parallel private(file) default(shared)
  {
#pragma omp master
    cout << "using " << omp_get_num_threads()
        << " threads for distance calculations" << endl;
#pragma omp for
    for (int i = numatoms - 1; i > 0; --i)
    {
      // iterate from j-1 to 0
      for (int j = i - 1; j >= 0; --j)
      {
        double distance = getDistance(atoms[i], atoms[j]);

        if (distance <= cutoff)
        {
          file = bonds->getFile(atoms[i].type, atoms[j].type);
          file->write(distance);
        }
      }
    }
  }
}

void printAtomTypes(const TypeVec &types)
{
  for (TypeVec::const_iterator it = types.begin(); it != types.end(); ++it)
  {
    out << it->name << "\t" << it->count << endl;
  }
}

ArgumentParser args;

void initArgs()
{
  args.Double("cutoff", "cutoff distance (type-independent)", 'c');

  args.Standalones(
      1,
      "input.xyz",
      "xyz file containing the atomic positions to analyze. Please note that only the first dataset of the file is considered.");
}

int main(int argc, char **argv)
{
  initArgs();
  args.parseArgs(argc, argv);

  TypeVec types;
  Atom *atoms;

  if (!args.allValuesSet("ERROR: %s not set\n"))
  {
    exit(1);
  }

  if (args.getStandaloneCount() != 1)
  {
    cerr << "no input file. See --help for help" << endl;
    exit(1);
  }

  const char *filename = args.getCStandalone(0);
  const double cutoff = args.getDouble("cutoff");

  cout << "using cutoff of " << cutoff << endl;
  cout << "reading file '" << filename << "'" << endl;

  int numatoms = readFile(filename, &types, &atoms);

  cout << "printing type info (name, number of atoms)" << endl;

  printAtomTypes(types);

  cout << "creating bond map" << endl;

  BondMap bonds(types);

  cout << "writing interatomic distances to files. Please be patient." << endl;
  printInteratomicDistances(atoms, numatoms, cutoff, &bonds);

  cout << "done" << endl;

  return 0;
}
