/*Include Headers-------------------------------------------------------------*/

/*Include user-defined headers------------------------------------------------*/
#include <MIP_main.h>
/*Extern variables------------------------------------------------------------*/

/*Static variables------------------------------------------------------------*/

/*Global variables------------------------------------------------------------*/

/*Functios--------------------------------------------------------------------*/
int main(int argc, char **argv) {
  char *method = argv[1];
  char *code = argv[2];
  int exitcode = -1;
  if (argc > 1) {
    if (!strcmp(method, "coder")) {
      /*Coder*/
      if (code == NULL) {
        fprintf(stderr, "Parameter not found! [coder]\n");
        return helper();
      }
      if ((exitcode = coder(atoi(code))) == 255) {
        fprintf(stderr, "Code invalid [coder]\n");
        return helper();
      } else {
        return exitcode;
      }
    } else if (!strcmp(method, "decoder")) {
      /*Decoder*/
      if (code == NULL) {
        fprintf(stderr, "Parameter not found! [decoder]\n");
        return helper();
      }
      if ((exitcode = decoder(atoi(code))) == 255) {
        fprintf(stderr, "Code invalid [decoder]\n");
        return helper();
      } else {
        return exitcode;
      }
    }
  }
  return helper();
}
/*----------------------------------------------------------------------------*/
int helper() {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  mip coder\n");
  fprintf(stderr, "  mip decoder\n");
  return 0;
}
/*----------------------------------------------------------------------------*/
