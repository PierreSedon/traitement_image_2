#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

#ifndef M_PI
    #define M_PI acos(-1.0)
#endif

void process(float tx, float ax, float ty, float ay, char *ims_name, char *imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      for (size_t k = 0; k < 3; k++)
      {
        float oldxcoord = j + ax*sin((2*M_PI*i)/tx);
        float oldycoord = i + ay*sin((2*M_PI*j)/ty);
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
  fprintf(stderr, "%s <tx> <ax> <ty> <ay> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 6
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);
  float tx = atof(argv[1]);
  float ax = atof(argv[2]);
  float ty = atof(argv[3]);
  float ay = atof(argv[4]);
  char *ims_name = argv[5];
  char *imd_name = argv[6];
  process(tx, ax, ty, ay, ims_name, imd_name);

  return EXIT_SUCCESS;
}
