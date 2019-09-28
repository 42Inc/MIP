/*Include Headers-------------------------------------------------------------*/

/*Include user-defined headers------------------------------------------------*/
#include <MIP_decoder.h>
/*Extern variables------------------------------------------------------------*/
extern const char zero_byte;
extern const char zero_char;
extern const char one_char;
extern char *filename;
/*Static variables------------------------------------------------------------*/

/*Global variables------------------------------------------------------------*/

/*Functios--------------------------------------------------------------------*/
int decoder(int code) {
  switch (code) {
  case 1:
    return fi0_decoder();
    break;
  case 2:
    return fi1_decoder();
    break;
  case 3:
    return fi2_decoder();
    break;
  default:
    return 255;
  }
}
/*----------------------------------------------------------------------------*/
int fi0_decoder() {
  char c = 0;
  int return_code = 0;
  unsigned int write_num = 0;
  char *out_filename = filename ? filename : "a.decode";
  FILE *out_fd = NULL;
  int i = 0;
  if (filename) {
    if ((out_fd = fopen(out_filename, "w")) == NULL) {
      fprintf(stderr, "File open error! [decoder]\n");
      return 1;
    }
  } else {
    out_fd = stdout;
  }
  do {
    return_code = read(STDIN_FILENO, &c, 1);
    if (c == 0) {
      write_num += 8;
    } else {
      for (i = 0; i < 8; ++i) {
        if ((c & 0x80) == 0) {
          write_num++;
        } else {
          fprintf(stderr, "Write num - %u [decoder]\n", write_num);
          fprintf(out_fd, "%u ", write_num);
          write_num = 0;
        }
        c = c << 1;
      }
    }
  } while (return_code != 0);
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi1_decoder() {
  char c = 0;
  int return_code = 0;
  char *out_filename = filename ? filename : "a.decode";
  FILE *out_fd = NULL;
  if (filename) {
    if ((out_fd = fopen(out_filename, "w")) == NULL) {
      fprintf(stderr, "File open error! [decoder]\n");
      return 1;
    }
  } else {
    out_fd = stdout;
  }
  do {
    return_code = read(STDIN_FILENO, &c, 1);
  } while (return_code != 0);
  return 0;
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi2_decoder() {
  char c = 0;
  int return_code = 0;
  char *out_filename = filename ? filename : "a.decode";
  FILE *out_fd = NULL;
  if (filename) {
    if ((out_fd = fopen(out_filename, "w")) == NULL) {
      fprintf(stderr, "File open error! [decoder]\n");
      return 1;
    }
  } else {
    out_fd = stdout;
  }
  do {
    return_code = read(STDIN_FILENO, &c, 1);
  } while (return_code != 0);
  return 0;
}
/*----------------------------------------------------------------------------*/
