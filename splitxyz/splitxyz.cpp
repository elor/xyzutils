#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

int main(int argc, char **argv)
{
  if (argc <= 2)
  {
    cerr << "usage: " << argv[0] << " <file.xyz> [prefix] [stepsize] [firststep] [digits] [maxfiles]" << endl;
    return 1;
  }

  const char *filename = argv[1];
  char prefix[1024];
  strcpy(prefix, filename);
  size_t stepsize = 1;
  size_t firststep = 1;
  bool usemaxfiles = false;
  size_t maxfiles = 0;
  size_t digits = 3;

  if (argc >= 3)
  {
    strcpy(prefix, argv[2]);
  }
  if (argc >= 4)
  {
    stepsize = atoi(argv[3]);
  }
  if (argc >= 5)
  {
    firststep = atoi(argv[4]);
  }
  if (argc >= 6)
  {
    digits = atoi(argv[5]);
  }
  if (argc >= 7)
  {
    maxfiles = atoi(argv[6]);
    usemaxfiles = true;
  }

  ifstream file(filename);

  size_t atomsleft = 0;
  size_t step = 0;
  size_t actualstep = 0;
  char prezero[16];
  char line[1024];
  char outfilename[1024];

  ofstream out;

  while(!file.eof())
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

          size_t n, i;

          memset(prezero, '0', 16);
          for (i = 1, n = step; n >= 10; ++i, n/=10);

          n = digits - i;
          if (n < 0 || n >= 16)
          {
            n = 0;
          }

          prezero[n] = '\0';

          sprintf(outfilename, "%s%s%lu.xyz", prefix, prezero, step);
          out.open(outfilename);
          cout << "writing step " << step << " to file " << outfilename << endl;
        }

        ++actualstep;
      }
      else
      {
        --firststep;
      }

      if (!out.is_open())
      {
        cout << "ignoring step " << step << endl;
      }
    }

    if (out.is_open())
    {
      out << line << endl;
    }
    --atomsleft;
  }

  return 0;
}

