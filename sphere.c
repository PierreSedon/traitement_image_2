#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

#ifndef M_PI
#define M_PI acos(-1.0)
#endif

void process(char *ims_name, char *imd_name)
{
    pnm ims = pnm_load(ims_name);
    int rows = pnm_get_height(ims);
    int cols = pnm_get_width(ims);

    int radius, diameter;
    if (rows < cols)
    {
        diameter = cols;
    }
    else
    {
        diameter = rows;
    }
    radius = diameter / 2;
    float sphereRadius = diameter / M_PI;

    int xMid = cols / 2;
    int yMid = rows / 2;

    pnm imd = pnm_new(cols, rows, PnmRawPpm);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            float lon = asin((j - xMid) * 1.0 / radius);
            float lat = asin((i - yMid) * 1.0 / (radius * cos(lon)));
            float oldxcoord = xMid + sphereRadius * lon;
            float oldycoord = yMid + sphereRadius * lat;
            if ((oldxcoord > 0) && (oldxcoord <= cols - 1) && (oldycoord > 0) && (oldycoord <= rows - 1))
            {
                for (size_t k = 0; k < 3; k++)
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
