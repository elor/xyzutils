/**
 * This class is an reader for the extended xyz file format's comments.
 * However, it concentrates on properties I find useful for my progs
 */
#include <vector>
#include <string>
#include <map>

using namespace std;

class excr
{
protected:
  excr();
  void createMap(char *input);
  vector<double> split(const char *str);

public:
  typedef map<string, string> smap;

  double xsize, ysize;
  double height;
  char *comment;
  int cycle;
  smap props;

  excr(char *input);
  ~excr();
};
