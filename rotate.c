#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

#ifndef M_PI
    #define M_PI acos(-1.0)
#endif

void process(int x, int y, float teta, char *ims_name, char *imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  float tetarad = (M_PI / 180) * teta;

  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      for (size_t k = 0; k < 3; k++)
      {
        float oldxcoord = cos(tetarad)*(j - x) + sin(tetarad)*(y - i) + x;
        float oldycoord = cos(tetarad)*(i - y) + sin(tetarad)*(j - x) + y;
        if ((oldxcoord < 0) || (oldxcoord >= cols - 1) || (oldycoord < 0) || (oldycoord >= rows - 1))
        {
          pnm_set_component(imd, i, j, k, 0);
        }
        else
        {
          unsigned short val = bilinear_interpolation(oldxcoord, oldycoord, ims, k);
          pnm_set_component(imd, i, j, k, val);
        }
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
}

static void usage(char *s)
{
  fprintf(stderr, "%s <x> <y> <angle> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 5
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);
  int x = atoi(argv[1]);
  int y = atoi(argv[2]);
  float teta = atof(argv[3]);
  char *ims_name = argv[4];
  char *imd_name = argv[5];
  process(x, y, teta, ims_name, imd_name);

  return EXIT_SUCCESS;
}
