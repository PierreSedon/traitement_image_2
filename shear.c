#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

#ifndef M_PI
    #define M_PI acos(-1.0)
#endif

void process(char dir, float teta, char *ims_name, char *imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  float tetarad = (M_PI / 180) * teta;
  int newcols, newrows;
  if (dir == 'h')
  {
    newrows = rows;
    newcols = floor(cols + rows * fabsf(tan(tetarad))) + 1;
  }
  else
  {
    newcols = cols;
    newrows = floor(rows + cols * fabsf(tan(tetarad))) + 1;
  }
  pnm imd = pnm_new(newcols, newrows, PnmRawPpm);

  float shear_factor = tan(tetarad);

  for (int i = 0; i < newrows; i++)
  {
    for (int j = 0; j < newcols; j++)
    {
      for (size_t k = 0; k < 3; k++)
      {
        if (dir == 'h')
        {
          float oldxcoord = j - shear_factor * i;
          if (teta < 0)
            oldxcoord -= rows * fabsf(tan(tetarad));
          if ((oldxcoord < 0) || (oldxcoord >= cols - 1) || (i >= rows - 1))
          {
            pnm_set_component(imd, i, j, k, 0);
          }
          else
          {
            unsigned short val = bilinear_interpolation(oldxcoord, i, ims, k);
            pnm_set_component(imd, i, j, k, val);
          }
        }
        else
        {
          float oldycoord = i - shear_factor * j;
          if (teta < 0)
            oldycoord -= cols * fabsf(tan(tetarad));
          if ((oldycoord < 0) || (oldycoord >= rows - 1) || (j >= cols - 1))
          {
            pnm_set_component(imd, i, j, k, 0);
          }
          else
          {
            unsigned short val = bilinear_interpolation(j, oldycoord, ims, k);
            pnm_set_component(imd, i, j, k, val);
          }
        }
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
}

static void usage(char *s)
{
  fprintf(stderr, "%s <dir>{h ,v} <angle> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);
  char dir = *argv[1];
  float teta = atof(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  process(dir, teta, ims_name, imd_name);

  return EXIT_SUCCESS;
}
