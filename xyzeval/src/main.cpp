#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <Atom.hpp>
#include <BondMap.hpp>
#include <TypeVec.hpp>
#include <ArgumentParser.h>
#include <CoordinationHistogramData.hpp>
#include <SquareSelector.hpp>
#include <utils.hpp>
#include <excr.hpp>
#include <stdexcept>
#include <convert.hpp>

using namespace std;

ofstream out("out_types.txt");

int readFile(const char *filename, TypeVec *out_types, Atom **out_atoms,
  double **spaceSize)
{
  ifstream file(filename);
  bool readSpecies = out_types->size() == 0;

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

  excr *commentData = NULL;
  try
  {
    commentData = new excr(comment);
  } catch (runtime_error&)
  {
    commentData = NULL;
  }

  if (commentData)
  {
    *spaceSize = new double[2];
    (*spaceSize)[0] = commentData->xsize;
    (*spaceSize)[1] = commentData->ysize;
  } else
  {
    *spaceSize = NULL;
    cout
      << "XYZ Comment doesn't contain spatial size. Assuming nonperiodic space."
      << endl;
  }

  double min[3] = { 0.0, 0.0, 0.0 };
  double max[3] = { 0.0, 0.0, 0.0 };
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
    } else
    {
      for (int k = 0; k < 3; ++k)
      {
        if (min[k] > atoms[i].pos[k])
        {
          min[k] = atoms[i].pos[k];
        } else if (max[k] < atoms[i].pos[k])
        {
          max[k] = atoms[i].pos[k];
        }
      }
    }

    for (TypeVec::iterator it = out_types->begin();; ++it)
    {
      if (it == out_types->end())
      {
        if (readSpecies)
        {
          out_types->push_back(Type(type, out_types->size()));
        } else
        {
          cerr << "unspecified species: " << type
            << ". Please update your species list." << endl
            << "For automatic detection, discard it along with coordinationCutoffs."
            << endl;
          exit(1);
        }
        break;
      } else if (it->name.compare(type) == 0)
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

double getDistance(const Atom &a, const Atom &b, double *spaceSize)
{
  double sum = 0.0;
  double tmp;

  for (int i = 0; i < 3; ++i)
  {
    tmp = abs(a.pos[i] - b.pos[i]);

    // minimum boundary condition
    if (i != 2 && spaceSize && tmp > spaceSize[i] * 0.5)
    {
      tmp -= spaceSize[i];
    }

    sum += tmp * tmp;
  }

  return sqrt(sum);
}

double getCutoff(size_t type1, size_t type2)
{
  // TODO read from external file or cli
  switch (type1)
  {
  case 0:
    switch (type2)
    {
    case 0:
      return 3.7;
    case 1:
      return 2.2;
    default:
      break;
    }
    break;
  case 1:
    switch (type2)
    {
    case 0:
      return 2.2;
    case 1:
      return 3.2;
    default:
      break;
    }
    break;
  default:
    break;
  }

  return 0.0;
}

void writeCoordination(CoordinationHistogramData &coords, const TypeVec &types)
{
  ofstream coordsfile("coords.txt");

  for (size_t type1 = 0; type1 < types.size(); ++type1)
  {
    for (size_t type2 = 0; type2 < types.size(); ++type2)
    {
      coordsfile << types[type1].name.c_str() << "-"
        << types[type2].name.c_str() << "\t";

      CoordinationHistogramData::Bins bins = coords.data[coords.getIndex(type1,
        type2)];
      for (CoordinationHistogramData::Bins::iterator it = bins.begin();
        it != bins.end(); ++it)
      {
        coordsfile << *it << " ";
      }
      coordsfile << endl;
    }
  }
}

void writeInteratomics(Atom *atoms, int numatoms, const double globalCutoff,
  const TypeVec &types, double *spaceSize,
  const SquareSelector<double> &coordCutoffs)
{
  // iterate from numatoms to 1

  BondMap bonds(types);
  CoordinationHistogramData coords(types.size());

  Bond *file;

  size_t *count = new size_t[types.size()];

  for (int i = numatoms - 1; i > 0; --i)
  {
    memset(count, '\0', sizeof(size_t) * types.size());

    // iterate from j-1 to 0
    for (int j = numatoms - 1; j >= 0; --j)
    {
      if (i == j)
      {
        continue;
      }

      double distance = getDistance(atoms[i], atoms[j], spaceSize);

      if (distance <= globalCutoff)
      {
        file = bonds.getFile(atoms[i].type, atoms[j].type);
        file->write(distance);
      }

      if (!coordCutoffs.empty()
        && distance <= coordCutoffs.select(atoms[i].type, atoms[j].type))
      {
        count[atoms[j].type] += 1;
      }
    }

    if (!coordCutoffs.empty())
    {
      for (size_t j = 0; j < types.size(); ++j)
      {
        coords.addValue(atoms[i].type, j, count[j]);
      }
    }
  }

  if (!coordCutoffs.empty())
  {
    writeCoordination(coords, types);
  }

  delete[] count;
}

void saveAtomTypes(const TypeVec &types)
{
  for (TypeVec::const_iterator it = types.begin(); it != types.end(); ++it)
  {
    out << it->name << "\t" << it->count << endl;
  }
}

ArgumentParser args;

void initArgs()
{
  args.Double("cutoff", 10.0, "global cutoff distance", 'c');
  args.String("species", "",
    "atomic species. Optional, but necessary for specific interatomic cutoffs",
    's');
  args.String("coordinationCutoffs", "",
    "cutoff distances between types. Handled as a line-representation of a matrix, i.e. species \"A B\" results in \"AA AB BA BB\". Requires species to be set.",
    'C');

  args.Standalones(1, "input.xyz",
    "xyz file containing the atomic positions to analyze. Please note that only the first dataset of the file is considered.");
}

SquareSelector<double> getCoordCutoffs(const TypeVec &types)
{
  vector<string> cutstrs = strsplit(args.getCString("coordinationCutoffs"));

  if (cutstrs.size() != 0)
  {
    if (types.size() == 0)
    {
      cerr << "coordinationCutoffs: species not defined" << endl;
      exit(1);
    }
    if (cutstrs.size() < types.size() * types.size())
    {
      cerr
        << "coordinationCutoffs: too few values. Must be number of species squared"
        << endl;
      exit(1);
    }
    if (cutstrs.size() > types.size() * types.size())
    {
      cerr
        << "coordinationCutoffs: too many values. Must be number of species squared"
        << endl;
      exit(1);
    }

    vector<double> cutoffs(cutstrs.size());
    for (size_t i = 0; i < cutstrs.size(); ++i)
    {
      double value;
      if (convert(cutstrs[i].c_str(), &value))
      {
        cerr << "coordinationCutoffs: invalid double value '"
          << cutstrs[i].c_str() << "'" << endl;
        exit(1);
      }

      cutoffs[i] = value;
    }
    return SquareSelector<double>(cutoffs);
  }

  return SquareSelector<double>(vector<double>());
}

TypeVec getTypes()
{
  TypeVec types;
  // init types with predefined species (if any)
  vector<string> species = strsplit(args.getCString("species"));
  for (size_t i = 0; i < species.size(); ++i)
  {
    types.push_back(Type(species[i].c_str(), types.size()));
  }

  return types;
}

int main(int argc, char **argv)
{
  initArgs();
  args.parseArgs(argc, argv);

  TypeVec types = getTypes();
  SquareSelector<double> coordCutoffs = getCoordCutoffs(types);
  Atom *atoms;

  if (args.getStandaloneCount() != 1)
  {
    cerr << "no input file. See --help for help" << endl;
    exit(1);
  }

  const char *filename = args.getCStandalone(0);
  const double globalCutoff = args.getDouble("cutoff");

  double *spaceSize = NULL;

  int numatoms = readFile(filename, &types, &atoms, &spaceSize);
  saveAtomTypes(types);
  writeInteratomics(atoms, numatoms, globalCutoff, types, spaceSize,
    coordCutoffs);

  if (spaceSize)
  {
    delete[] spaceSize;
    spaceSize = NULL;
  }

  return 0;
}
