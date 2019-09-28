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
  int index = 0;
  int return_code = 0;
  char *out_filename = filename ? filename : "a.code";
  int out_fd = -1;
  int mod = 0;
  int div = 0;
  char write_byte = 0;
  char write_byte_index = 0;
  char full_byte_flag = 0;
  char from_last_byte = 0;
  char from_last_byte_flag = 1;
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
      number = number * (pow(10, index)) + (c - '0');
      ++index;
    } else {
      if (read_num) {
        fprintf(stderr, "Read num - %d [coder]\n", number);
      /* Put coder process here */
      label_write_byte:
        if (full_byte_flag) {
          write(out_fd, &write_byte, 1);
          write_byte = 0;
          write_byte_index = 0;
          full_byte_flag = 0;
          from_last_byte_flag = 0;
        }
        if (from_last_byte - 1 >= number) {
          write_byte_index = write_byte_index - number - 1;
          write_byte = write_byte | (1 << write_byte_index);
          from_last_byte = from_last_byte - number - 1;
          if (from_last_byte < 0) {
            from_last_byte = 0;
          }
          if (from_last_byte == 0) {
            full_byte_flag = 1;
          }
          goto label_end_coder;
        } else if (from_last_byte > 0 && from_last_byte_flag) {
          full_byte_flag = 1;
          goto label_write_byte;
        }
        from_last_byte_flag = 1;
        div = (number - from_last_byte) / 8;
        mod = (number - from_last_byte) % 8;
        from_last_byte = 0;
        for (i = 0; i < div; ++i) {
          write(out_fd, &zero_byte, 1); // write zero_byte_byte
        }
        write_byte_index = 8 - mod - 1;
        write_byte = write_byte | (1 << write_byte_index);
        if (write_byte_index > 0) {
          from_last_byte = write_byte_index;
        }
      label_end_coder:
        read_num = 0;
        number = 0;
        index = 0;
      }
    }
  } while (return_code != 0);
  // write last byte
  write(out_fd, &write_byte, 1);
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi1_coder() {
  char c = 0;
  int read_num = 0;
  int number = 0;
  int binary_length = 0;
  int write_bits = 0;
  int index = 0;
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
      number = number * (pow(10, index)) + (c - '0');
      ++index;
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
          compressor(zero_char, out_fd);
          //          write(out_fd, &zero_char, 1);
        }
        compressor(one_char, out_fd);
        //        write(out_fd, &one_char, 1);
        for (i = binary_length - 2; i >= 0; --i) {
          if ((number >> i) & 0x1) {
            compressor(one_char, out_fd);
            //            write(out_fd, &one_char, 1);
          } else {
            compressor(zero_char, out_fd);
            //            write(out_fd, &zero_char, 1);
          }
        }
        read_num = 0;
        number = 0;
        index = 0;
      }
    }
  } while (return_code != 0);
  compressor('-', out_fd);
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi2_coder() {
  char c = 0;
  int read_num = 0;
  int number = 0;
  int index = 0;
  int return_code = 0;
  char *out_filename = filename ? filename : "a.code";
  int out_fd = -1;
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
      number = number * (pow(10, index)) + (c - '0');
      ++index;
    } else {
      if (read_num) {
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
int get_binary_length(int num) {
  unsigned int result = 0;
  if (num == 0)
    return 1;
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
  if (sym == '-') {
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
