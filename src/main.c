/*Include Headers-------------------------------------------------------------*/

/*Include user-defined headers------------------------------------------------*/
#include <MIP_main.h>
/*Extern variables------------------------------------------------------------*/

/*Static variables------------------------------------------------------------*/

/*Global variables------------------------------------------------------------*/
const char zero = 0;
char *filename = NULL;
/*Functios--------------------------------------------------------------------*/
int main(int argc, char **argv) {
  char *method = argv[1];
  char *code = argv[2];
  filename = argv[3];
  int exit_code = -1;
  if (argc > 1) {
    if (!strcmp(method, "coder")) {
      /*Coder*/
      if (code == NULL) {
        fprintf(stderr, "Parameter not found! [coder]\n");
        return helper();
      }
      if ((exit_code = coder(atoi(code))) == 255) {
        fprintf(stderr, "Code invalid [coder]\n");
        return helper();
      } else {
        return exit_code;
      }
    } else if (!strcmp(method, "decoder")) {
      /*Decoder*/
      if (code == NULL) {
        fprintf(stderr, "Parameter not found! [decoder]\n");
        return helper();
      }
      if ((exit_code = decoder(atoi(code))) == 255) {
        fprintf(stderr, "Code invalid [decoder]\n");
        return helper();
      } else {
        return exit_code;
      }
    }
  }
  return helper();
}
/*----------------------------------------------------------------------------*/
int helper() {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  mip coder <CODE>\n");
  fprintf(stderr, "  mip decoder <CODE>\n");
  return 0;
}
/*----------------------------------------------------------------------------*/
