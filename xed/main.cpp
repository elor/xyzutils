// this processor is NOT inline!

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <string>
#include <convert.hpp>

using namespace std;

class Atom
{
public:
  double pos[3];
  char type[32];

  void parse(const char *source)
  {
    if (sscanf(source, "%s %lf %lf %lf", type, &pos[0], &pos[1], &pos[2]) != 4)
    {
      cerr << "error while parsing atom line: " << source << endl;
      throw runtime_error("error while parsing atom line");
    }
  }
};

std::ostream &operator<<(std::ostream &os, const Atom &atom)
{
  os << atom.type << " " << atom.pos[0] << " " << atom.pos[1] << " " << atom.pos[2];
  return os;
}

class AtomSet
{
public:
  typedef vector<Atom> Vector;
  Vector atoms;
  char comment[1024];

  static AtomSet create(istream &in)
  {
    if (in.eof())
    {
      throw runtime_error("can't read from input stream");
    }

    AtomSet atomSet;

    char buffer[1024];
    in.getline(buffer, 1024);
    unsigned int numatoms = 0;

    if (convert(buffer, &numatoms))
    {
      cerr << "corrupt atom count line: " << buffer << endl;
      throw runtime_error("corrupt atom count line");
    }

    if (in.eof())
    {
      throw runtime_error("unexpected end of file");
    }

    in.getline(atomSet.comment, 1024);

    Atom tmpAtom;

    for (size_t i = 0; i < numatoms; ++i)
    {
      if (in.eof())
      {
        throw runtime_error("unexpected end of file");
      }

      in.getline(buffer, 1024);

      tmpAtom.parse(buffer);

      atomSet.atoms.push_back(tmpAtom);
    }

    return atomSet;
  }
};

std::ostream &operator<<(std::ostream &os, const AtomSet &atomSet)
{
  os << atomSet.atoms.size() << endl;
  os << atomSet.comment << endl;

  for (AtomSet::Vector::const_iterator it = atomSet.atoms.begin(); it != atomSet.atoms.end(); ++it)
  {
    os << *it << endl;
  }

  return os;
}

std::ostream &operator<<(std::ostream &os, const AtomSet *atomSet)
{
  os << *atomSet;
  return os;
}

class Selector
{
public:
  enum {
    x = 0,  // x/,/
    y = 1,  // y/,/
    z = 2 // z/,/
  } axis;

  double min;
  double max;

  static const double any;

  bool operator()(const Atom &atom)
  {
    return (min == any || atom.pos[axis] >= min) && (max == any || atom.pos[axis] <= max);
  }
};

unsigned long long deadbeef = 0xdeadbeef;
const double Selector::any = *reinterpret_cast<const double*>(&deadbeef);

class Command
{
public:
  Selector selector;

  enum {
    print,  // prints if selected, removes otherwise, 'p'
    remove  // removes if selected, prints otherwise, 'd'
  } action;

  int operator()(const Atom &original, Atom* out)
  {
    bool selected = selector(original);

    switch (action)
    {
    case print:
      if (!selected)
      {
        return 1;
      }

      *out = original;
      return 0;
    case remove:
      if (selected)
      {
        return 1;
      }

      *out = original;
      return 0;
    default:
      throw runtime_error("unknown command during execution");
    }

    throw runtime_error("reached invalid part of the code during command execution");
  }

  AtomSet operator()(const AtomSet &in)
  {
    AtomSet out;
    strcpy(out.comment, in.comment);

    Atom tmpAtom;

    for (AtomSet::Vector::const_iterator it = in.atoms.begin(); it != in.atoms.end(); ++it)
    {
      if ((*this)(*it, &tmpAtom) == 0)
      {
        out.atoms.push_back(tmpAtom);
      }
    }
    
    return out;
  }
};

Command parseCommand(const char* str){
  Command command;

  if (strlen(str) < 7)
  {
    cerr << "Command too short: '" << str << "'" << endl;
    throw runtime_error("command too short");
  }

  switch (str[0])
  {
  case 'x':
    command.selector.axis = Selector::x;
    break;
  case 'y':
    command.selector.axis = Selector::y;
    break;
  case 'z':
    command.selector.axis = Selector::z;
    break;
  default:
    cerr << "unknown selector: '" << str[0] << endl;
    throw runtime_error("unknown selector");
  }

  if (str[1] != '/')
  {
    cerr << "Syntax Error: '" << str << "'" << endl;
    throw runtime_error("syntax error");
  }

  if (str[2] == '*')
  {
    if (str[3] != ',')
    {
      cerr << "unexpected symbol after selector minimum value" << endl;
      throw runtime_error("unexpected symbol after selector minimum value");
    }
    command.selector.min = Selector::any;
  }
  else if (sscanf(&str[2], "%lf,", &command.selector.min) != 1)
  {
    cerr << "invalid value for selector minimum value: " << str << endl;
    throw runtime_error("invalid value for selector minimum value");
  }

  size_t i = 3;
  while (str[i] != ',')
  {
    if (str[i] == '\0')
    {
      cerr << "unexpected end of command: " << str << endl;
      throw runtime_error("unexpected end of command");
    }

    ++i;
  }

  ++i;

  if (str[i] == '*')
  {
    if (str[i+1] != '/')
    {
      cerr << "unexpected symbol after selector maximum value" << endl;
      throw runtime_error("unexpected symbol after selector maximum value");
    }
    command.selector.max = Selector::any;
  }
  else if (sscanf(&str[i], "%lf/", &command.selector.max) != 1)
  {
    cerr << "invalid value for selector maximum value: " << str << endl;
    throw runtime_error("invalid value for selector maximum value");
  }

  i = 3;
  while (str[i] != '/')
  {
    if (str[i] == '\0')
    {
      cerr << "unexpected end of command: " << str << endl;
      throw runtime_error("unexpected end of command");
    }

    ++i;
  }

  ++i;
  if (str[i+1] != '\0')
  {
    cerr << "unexpected symbols after command character: " << str << endl;
    throw runtime_error("unexpected symbols after command character");
  }

  switch (str[i])
  {
  case 'p':
    command.action = Command::print;
    break;
  case 'd':
    command.action = Command::remove;
    break;
  default:
    cerr << "unknown command character: " << str << endl;
    throw runtime_error("unknown command character");
  }

  return command;
}

int main(int argc, char **argv)
{
  if (argc == 1)
  {
    throw runtime_error("no command given");
  }

  // create commands
  size_t numcommands = argc - 1;
  Command *commands = new Command[numcommands];

  for (size_t i = 0; i < numcommands; ++i)
  {
    commands[i] = parseCommand(argv[i+1]);
  }

  // create atom set
  AtomSet firstSet = AtomSet::create(cin);
  AtomSet secondSet;

  AtomSet *in = &firstSet;
  AtomSet *out = &secondSet;
  AtomSet *final = NULL;

  for (size_t i = 0; i < numcommands; ++i)
  {
    *out = commands[i](*in);

    final = out;
    out = in;
    in = final;
  }

  // processing
  delete[] commands;

  // output
  cout << final << endl;
}

