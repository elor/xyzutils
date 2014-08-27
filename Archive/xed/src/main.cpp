// TODO: inlinify

#include <Command.hpp>

#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std;

Command parseCommand(const char* str)
{
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
    if (str[i + 1] != '/')
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
  if (str[i + 1] != '\0')
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
    commands[i] = parseCommand(argv[i + 1]);
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

  return 0;
}

