#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ArgumentParser.h>

using namespace std;

ArgumentParser args;

void initArgs(int argc, char **argv)
{
  args.String("prefix", "", "filename prefix for output files", 'p');
  args.UInt("step", 1, "step size between xyz frames.", 's');
  args.UInt("first", 1, "first step to print", 'f');
  args.UInt("digits", 0, "number of digits for output filename", 'd');
  args.UInt("max", 0, "maximum number of output files. 0 = infinite", 'm');
  args.Standalones(
      1,
      "input.xyz",
      "input file in xyz format. Please note that xyz files aren't properly validated prior to splitting.");

  args.parseArgs(argc, argv);

  if (args.getStandaloneCount() != 1)
  {
    args.displayHelpMessage();
    exit(1);
  }
}

int main(int argc, char **argv)
{
  initArgs(argc, argv);

  //        << " <file.xyz> [prefix] [stepsize] [firststep] [digits] [maxfiles]"

  const char *prefix = args.getCString("prefix");
  size_t stepsize = args.getUInt("step");
  size_t firststep = args.getUInt("first");
  size_t digits = args.getUInt("digits");
  size_t maxfiles = args.getUInt("max");
  bool usemaxfiles = maxfiles != 0;
  const char *filename = args.getCStandalone(0);

  ifstream file(filename);

  size_t atomsleft = 0;
  size_t step = 0;
  size_t actualstep = 0;
  char line[1024];
  char outfilename[1024];

  ofstream out;

  while (!file.eof())
  {
    file.getline(line, 1024);

    if (atomsleft == 0)
    {
      if (file.eof())
      {
        cout << "end of file reached" << endl;
        break;
      }
      ++step;
      out.close();
      atomsleft = atoi(line) + 2;

      if (firststep <= 1)
      {
        if (actualstep % stepsize == 0)
        {
          if (usemaxfiles)
          {
            if (maxfiles == 0)
            {
              break;
            }
            --maxfiles;
          }

          sprintf(outfilename, "%s%0*lu.xyz", prefix, (int) digits, step);
          out.open(outfilename);
          cout << "writing step " << step << " to file " << outfilename << endl;
        }

        ++actualstep;
      }
      else
      {
        --firststep;
      }

      //      if (!out.is_open())
      //      {
      //        cout << "ignoring step " << step << endl;
      //      }
    }

    if (out.is_open())
    {
      out << line << endl;
    }
    --atomsleft;
  }

  return 0;
}

