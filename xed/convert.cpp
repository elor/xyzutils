#include <convert.hpp>
#include <cstdlib>
#include <cerrno>

int convert(const char *str, double *out)
{
  errno = 0;
  char *p;
  double tmp = strtod(str, &p);

  if (errno != 0 || str == p)
  {
    return 1;
  }

  *out = tmp;
  return 0;
}

int convert(const char *str, int *out)
{
  errno = 0;
  char *p;
  int tmp = strtol(str, &p, 10);

  if (errno != 0 || str == p)
  {
    return 1;
  }

  *out = tmp;
  return 0;
}

int convert(const char *str, unsigned int *out)
{
  errno = 0;
  char *p;
  unsigned int tmp = strtoul(str, &p, 10);

  if (errno != 0 || str == p)
  {
    return 1;
  }

  *out = tmp;
  return 0;
}

int convert(const char *str, unsigned long long *out)
{
  errno = 0;
  char *p;
  unsigned long long tmp = strtoull(str, &p, 10);

  if (errno != 0 || str == p)
  {
    return 1;
  }

  *out = tmp;
  return 0;
}
