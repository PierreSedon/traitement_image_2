#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

#ifndef M_PI
#define M_PI acos(-1.0)
#endif

void computeLatLong(int x, int y, int maxX, int maxY, float *lat, float *lon)
{
    int x_mid = maxX / 2;
    int y_mid = maxY / 2;
    // Compute latitude and longitude in degrees
    *lon = ((x - x_mid * 1.0) / x_mid) * 179.9;
    *lat = ((y - y_mid * 1.0) / y_mid) * -89.9;
    // Now convert to radian
    //*lon = (*lon) * M_PI / 180;
    //*lat = (*lat) * M_PI / 180;
}

void process(char *ims_name, char *imd_name)
{
    pnm ims = pnm_load(ims_name);
    int rows = pnm_get_height(ims);
    int cols = pnm_get_width(ims);

    int radius;
    if (rows < cols)
    {
        radius = cols;
    }
    else
    {
        radius = rows;
    }

    pnm imd = pnm_new(cols, rows, PnmRawPpm);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (size_t k = 0; k < 3; k++)
            {
                float x = (j - cols/2.0)/cols;
                float y = (i - rows/2.0)/rows;
                float xcoord3d = 2.0*x/(1.0+x*x+y*y);
                float ycoord3d = 2.0*y/(1.0+x*x+y*y);
                float zcoord3d = (-1.0+x*x+y*y)/(1.0+x*x+y*y);
                //printf("3d coord %f %f: x %f, y %f z %f\n", x, y, xcoord3d, ycoord3d, zcoord3d);
                float oldxcoord = (0.5 + atan2(xcoord3d, zcoord3d) / (2 * M_PI))*cols;
                float oldycoord = (0.5 - asin(ycoord3d / radius) / M_PI)*rows;
                printf("%d, %d : %f %f %d\n", i, j, oldxcoord, oldycoord, radius);
                if ((oldxcoord < 0) || (oldxcoord >= cols - 1) || (oldycoord < 0) || (oldycoord >= rows - 1))
                {
                    pnm_set_component(imd, i, j, k, 0);
                }
                else
                {
                    unsigned short val = bilinear_interpolation(oldxcoord, oldycoord, ims, k);
                    pnm_set_component(imd, i, j, k, val);
                }
                //}
            }
        }
    }

    pnm_save(imd, PnmRawPpm, imd_name);
}

static void usage(char *s)
{
    fprintf(stderr, "%s <ims> <imd>\n", s);
    exit(EXIT_FAILURE);
}

#define PARAM 2
int main(int argc, char *argv[])
{
    if (argc != PARAM + 1)
        usage(argv[0]);
    char *ims_name = argv[1];
    char *imd_name = argv[2];
    process(ims_name, imd_name);

    return EXIT_SUCCESS;
}
