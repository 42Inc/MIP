/*Include Headers-------------------------------------------------------------*/

/*Include user-defined headers------------------------------------------------*/
#include <MIP_coder.h>
/*Extern variables------------------------------------------------------------*/
extern const char zero_byte;
extern const char zero_char;
extern const char one_char;
extern char *filename;
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
  int return_code = 0;
  char *out_filename = filename ? filename : "a.code";
  int out_fd = -1;
  int i = 0;
  if (filename) {
    if (
      (out_fd = open(out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1) {
      fprintf(stderr, "File open error! [coder]\n");
      return 1;
    }
  } else {
    out_fd = STDOUT_FILENO;
  }
  do {
    return_code = read(STDIN_FILENO, &c, 1);
    if (is_num(c)) {
      read_num = 1;
      number = number * 10 + (c - '0');
    } else {
      if (read_num) {
        fprintf(stderr, "Read num - %d [coder]\n", number);
        /* Put coder process here */
        for (i = 0; i < number; ++i) {
          compressor(zero_char, out_fd);
        }
        compressor(one_char, out_fd);
        read_num = 0;
        number = 0;
      }
    }
  } while (return_code != 0);
  // write last byte
  compressor(zero_byte, out_fd);
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi1_coder() {
  char c = 0;
  int read_num = 0;
  int number = 0;
  int binary_length = 0;
  int write_bits = 0;
  int return_code = 0;
  char *out_filename = filename ? filename : "a.code";
  int out_fd = -1;
  int i = 0;
  if (filename) {
    if (
      (out_fd = open(out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1) {
      fprintf(stderr, "File open error! [coder]\n");
      return 1;
    }
  } else {
    out_fd = STDOUT_FILENO;
  }
  do {
    return_code = read(STDIN_FILENO, &c, 1);
    if (is_num(c)) {
      read_num = 1;
      number = number * 10 + (c - '0');
    } else {
      if (read_num) {
        binary_length = get_binary_length(number);
        write_bits = binary_length << 1;
        fprintf(
          stderr,
          "Read num - %d [%u]. Write - %u [coder]\n",
          number,
          binary_length,
          write_bits);
        /* Put coder process here */
        for (i = 0; i < binary_length; ++i) {
#if DEBUG == FALSE
          compressor(zero_char, out_fd);
#else
          write(out_fd, &zero_char, 1);
#endif
        }
#if DEBUG == FALSE
        compressor(one_char, out_fd);
#else
        write(out_fd, &one_char, 1);
#endif
        for (i = binary_length - 2; i >= 0; --i) {
          if ((number >> i) & 0x1) {
#if DEBUG == FALSE
            compressor(one_char, out_fd);
#else
            write(out_fd, &one_char, 1);
#endif
          } else {
#if DEBUG == FALSE
            compressor(zero_char, out_fd);
#else
            write(out_fd, &zero_char, 1);
#endif
          }
        }
        read_num = 0;
        number = 0;
      }
    }
  } while (return_code != 0);
#if DEBUG == FALSE
  compressor(zero_byte, out_fd);
#endif
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi2_coder() {
  char c = 0;
  int read_num = 0;
  int number = 0;
  int return_code = 0;
  char *out_filename = filename ? filename : "a.code";
  int out_fd = -1;
  int number_binary_length = 0;
  int binary_length = 0;
  int i = 0;
  if (filename) {
    if (
      (out_fd = open(out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1) {
      fprintf(stderr, "File open error! [coder]\n");
      return 1;
    }
  } else {
    out_fd = STDOUT_FILENO;
  }
  do {
    return_code = read(STDIN_FILENO, &c, 1);
    if (is_num(c)) {
      read_num = 1;
      number = number * 10 + (c - '0');
    } else {
      if (read_num) {
        number_binary_length = get_binary_length(number);
        binary_length = get_binary_length(number_binary_length);
        fprintf(
          stderr,
          "Read num - %d [coder] [%u | %u]\n",
          number,
          number_binary_length,
          binary_length);
        for (i = 0; i < binary_length; ++i) {
#if DEBUG == FALSE
          compressor(zero_char, out_fd);
#else
          write(out_fd, &zero_char, 1);
#endif
        }
#if DEBUG == FALSE
        compressor(one_char, out_fd);
#else
        write(out_fd, &one_char, 1);
#endif
        for (i = binary_length - 2; i >= 0; --i) {
          if ((number_binary_length >> i) & 0x1) {
#if DEBUG == FALSE
            compressor(one_char, out_fd);
#else
            write(out_fd, &one_char, 1);
#endif
          } else {
#if DEBUG == FALSE
            compressor(zero_char, out_fd);
#else
            write(out_fd, &zero_char, 1);
#endif
          }
        }
        for (i = number_binary_length - 2; i >= 0; --i) {
          if ((number >> i) & 0x1) {
#if DEBUG == FALSE
            compressor(one_char, out_fd);
#else
            write(out_fd, &one_char, 1);
#endif
          } else {
#if DEBUG == FALSE
            compressor(zero_char, out_fd);
#else
            write(out_fd, &zero_char, 1);
#endif
          }
        }
        read_num = 0;
        number = 0;
      }
    }
  } while (return_code != 0);
#if DEBUG == FALSE
  compressor(zero_byte, out_fd);
#endif
  return 0;
}
/*----------------------------------------------------------------------------*/
int get_binary_length(int num) {
  unsigned int result = 0;
  if (num == 0)
    return 0;
  while (num > 0) {
    num = num >> 1;
    ++result;
  }
  return result;
}
/*----------------------------------------------------------------------------*/
int compressor(char sym, int out_fd) {
  static int compressor_write_byte = 0;
  static int compressor_byte_index = 7;
  if (sym == zero_byte) {
    write(out_fd, &compressor_write_byte, 1);
    close(out_fd);
    return 0;
  }
  compressor_write_byte
    = compressor_write_byte | ((sym - '0') << compressor_byte_index);
  --compressor_byte_index;
  if (compressor_byte_index < 0) {
    write(out_fd, &compressor_write_byte, 1);
    compressor_byte_index = 7;
    compressor_write_byte = 0;
  }
  return 0;
}
/*----------------------------------------------------------------------------*/
