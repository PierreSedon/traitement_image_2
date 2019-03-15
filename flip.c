#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

void
process(char dir, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load( ims_name );
  int rows, cols;
  if (dir == 't'){
    cols = pnm_get_height(ims);
    rows = pnm_get_width(ims);
  } else {
    rows = pnm_get_height(ims);
    cols = pnm_get_width(ims);
  }

  pnm imd  = pnm_new(cols, rows, PnmRawPpm);

  for(int i=0; i < rows; i++){
    for(int j=0; j < cols; j++){
      for(size_t k=0; k<3; k++){
        unsigned short val;
        if (dir == 't'){
          val = pnm_get_component(ims, j, i, k);
        } else {
          val = pnm_get_component(ims, i, j, k);
        }

        switch(dir) {
          case 'h':
            pnm_set_component(imd, i, cols - 1 - j, k, val);
            break;
          case 'v':
            pnm_set_component(imd, rows - 1 - i, j, k, val);
            break;
          case 't':
            pnm_set_component(imd, i, j, k, val);
            break;
          default:
            break;
        }
      }
    }
  }
  pnm_save(imd, PnmRawPpm, imd_name);
}

void
usage(char* s)
{
  fprintf(stderr,"%s <dir> <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int
main(int argc, char* argv[])
{
  if(argc != PARAM+1)
    usage(argv[0]);
  char * dir = argv[1];
  char * ims_name = argv[2];
  char * imd_name = argv[3];
  int count = 0;
  while (dir[count] != '\0'){
    if ((dir[count] != 'h') && (dir[count] != 't') && (dir[count] != 'v')){
      fprintf(stderr,"Les directions autorisées sont 'h', 'v' et 't'\n");
      exit(EXIT_FAILURE);
    }
    if (count == 0){
      process(dir[count], ims_name, imd_name);
    } else {
      process(dir[count], imd_name, imd_name);
    }
    count++;
  }

  return EXIT_SUCCESS;
}
