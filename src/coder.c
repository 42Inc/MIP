/*Include Headers-------------------------------------------------------------*/

/*Include user-defined headers------------------------------------------------*/
#include <MIP_coder.h>
/*Extern variables------------------------------------------------------------*/

/*Static variables------------------------------------------------------------*/

/*Global variables------------------------------------------------------------*/

/*Functios--------------------------------------------------------------------*/
int coder(int code) {
  switch (code) {
  case 1:
    return fi0_coder();
    break;
  case 2:
    return fi1_coder();
    break;
  case 3:
    return fi2_coder();
    break;
  default:
    return 255;
  }
}
/*----------------------------------------------------------------------------*/
int fi0_coder() {
  char c = 0;
  int read_num = 0;
  int number = 0;
  int index = 0;
  int return_code = 0;
  do {
    return_code = read(STDIN_FILENO, &c, 1);
    if (is_num(c)) {
      read_num = 1;
      number = number * (10 * index++) + (c - '0');
    } else {
      if (read_num) {
        /* Put coder process here */
        fprintf(stderr, "Read num - %d [coder]\n", number);
        read_num = 0;
        number = 0;
        index = 0;
      }
    }
  } while (return_code != 0);
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi1_coder() {
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi2_coder() {
  return 0;
}
/*----------------------------------------------------------------------------*/
