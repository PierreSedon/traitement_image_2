#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

void
process(int dx, int dy, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load( ims_name );
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  pnm imd  = pnm_new(cols, rows, PnmRawPpm);

  for(int i=0; i < rows; i++){
    for(int j=0; j < cols; j++){
      for(size_t k=0; k<3; k++){
        const unsigned short val = pnm_get_component(ims, i, j, k);
        pnm_set_component(imd, (rows + i + dy)%rows, (cols + j + dx)%cols, k, val);
      }
    }
  }
  pnm_save(imd, PnmRawPpm, imd_name);
}

void
usage(char* s)
{
  fprintf(stderr,"%s <dx> <dy> <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int
main(int argc, char* argv[])
{
  if(argc != PARAM+1)
    usage(argv[0]);
  int dx = atoi(argv[1]);
  int dy = atoi(argv[2]);
  char * ims_name = argv[3];
  char * imd_name = argv[4];
  process(dx, dy, ims_name, imd_name);

  return EXIT_SUCCESS;
}
