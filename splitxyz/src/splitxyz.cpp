#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <ArgumentParser.h>

#ifndef BUFFERSIZE
  #define BUFFERSIZE 1024
#endif

using namespace std;

ArgumentParser args;

void initArgs(int argc, char **argv)
{
  args.String("output", "", "filename for output file(s). Leave empty for STDOUT. If it contains a series of '#', each step will be contained within its own file, where '#' is replaced with the step number", 'o');
  args.UInt("step", 1, "step size", 's');
  args.UInt("begin", 1, "first step to print. Starts at 1.", 'b');
  args.UInt("end", 0, "last step to print. 0 = infinite", 'e');
  args.Standalones(
      -1,
      "input.xyz",
      "input file in xyz format. Please note that xyz files aren't properly validated prior to splitting.");

  args.parseArgs(argc, argv);

  if (args.getStandaloneCount() < 0)
  {
    args.displayHelpMessage();
    exit(1);
  }
}

int main(int argc, char **argv)
{
  initArgs(argc, argv);

  char outfile_format[BUFFERSIZE];
  memset(outfile_format, '\0', BUFFERSIZE);
  const char *outfile_argument = args.getCString("output");
  bool useSTDOUT = outfile_argument == NULL || strlen(outfile_argument) == 0;

  size_t digits = 0;
  bool useMultipleFiles = false;
  if (!useSTDOUT)
  {
    size_t length = strlen(outfile_argument);
    for (size_t src = 0, trg = 0; src < length; ++src, ++trg)
    {
      if (outfile_argument[src] == '%')
      {
        cerr << "output filename must not contain '%'" << endl;
        exit(1);
      }

      while (outfile_argument[src] == '#')
      {
        if (!useMultipleFiles)
        {
          static const char *anchor = "%0*lu";
          strcpy(&outfile_format[trg], anchor);
          trg += strlen(anchor);
          useMultipleFiles = true;
        }
        ++digits;
        ++src;
      }
      
      outfile_format[trg] = outfile_argument[src];
    }
  }

  size_t stepsize = args.getUInt("step");
  if (stepsize == 0)
  {
    stepsize = 1;
  }
  size_t firststep = args.getUInt("begin");
  if (firststep == 0)
  {
    firststep = 1;
  }
  size_t laststep = args.getUInt("end");
  bool useLastStep = (laststep != 0);
  int numFiles = args.getStandaloneCount();
  bool useSTDIN = numFiles == 0;

  istream *input;

  size_t currentFile = 0;

  if (useSTDIN)
  {
    input = &cin;
  }
  else
  {
    input = new ifstream(args.getCStandalone(currentFile));
    assert(reinterpret_cast<ifstream*>(input)->is_open());
  }

  size_t atomsleft = 0;
  size_t step = 0;
  size_t actualstep = 0;
  char line[BUFFERSIZE];
  char outfilename[BUFFERSIZE];

  ostream *out = NULL;

  if (useSTDOUT)
  {
    out = &cout;
  }
  else if (!useMultipleFiles)
  {
    strcpy(outfilename, outfile_format);
    out = new ofstream(outfilename);
  }

  bool printLine = false;

  while (true)
  {
    if (input->eof())
    {
      if (useSTDIN)
      {
        break;
      }
      else
      {
        // select next input file
        delete input;
        input = NULL;
        ++currentFile;
        if (currentFile >= numFiles)
        {
          // last file reached;
          break;
        }
        input = new ifstream(args.getCStandalone(currentFile));
        assert(reinterpret_cast<ifstream*>(input)->is_open());
      }
    }

    input->getline(line, BUFFERSIZE);
    if (input->eof() && strlen(line) == 0)
    {
      // skip any empty lines at the end of a file
      continue;
    }

    if (atomsleft == 0)
    {
      printLine = false;

      ++step;
      if (useLastStep && step > laststep)
      {
        break;
      }

      if (useMultipleFiles && !useSTDOUT)
      {
        delete out;
        out = NULL;
      }
      atomsleft = atoi(line) + 2;

      if (step >= firststep)
      {
        if (actualstep % stepsize == 0)
        {
          if (useMultipleFiles)
          {
            assert(out == NULL);

//            cerr << outfile_format << endl;
//            cerr << digits << endl;

            sprintf(outfilename, outfile_format, (int) digits, step);
//            cerr << outfilename << endl;
            out = new ofstream(outfilename);
          }
//          cout << "writing step " << step << " to file " << outfilename << endl;
          printLine = true;
        }

        ++actualstep;
      }

      //      if (!out.is_open())
      //      {
      //        cout << "ignoring step " << step << endl;
      //      }
    }

    if (printLine)
    {
      assert(out != NULL);
      *out << line << endl;
    }
    --atomsleft;
  }

  if (out != NULL && !useSTDOUT)
  {
    delete out;
    out = NULL;
  }

  return 0;
}

