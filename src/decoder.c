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
      //      fprintf(stderr, "= %u [i = -]\n", write_num);
    } else {
      for (i = 0; i < 8; ++i) {
        if ((c & 0x80) == 0) {
          write_num++;
        } else {
          fprintf(stderr, "Write num - %u [decoder]\n", write_num);
          fprintf(out_fd, "%u ", write_num);
          write_num = 0;
        }
        //        fprintf(stderr, "= %u [i = %d]\n", write_num, i);
        c = c << 1;
      }
    }
  } while (return_code != 0);
  return 0;
}
/*----------------------------------------------------------------------------*/
int fi1_decoder() {
  int write_num = 0;
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
  /* clang-format off */
    __asm__ volatile(
  "DO_WHILE_DECODER_2_START:"
    "xorq %%rax, %%rax\n\t"
    "xorq %%rbx, %%rbx\n\t"
    "xorq %%rcx, %%rcx\n\t"
    "xorq %%rdx, %%rdx\n\t"
  "DO_WHILE_BIN_LEN%=:\n\t"
    "call decompressor\n\t"
    "cmpb $0, %%al\n\t"
    "jns C_BIN_LEN_ELSE_IF%=\n\t"
    "cmpl $0, %%ebx\n\t"
    "jne C_BIN_LEN_ELSE_IF%=\n\t"
    "movl $0, %%eax\n\t"
    "leave\n\t"
    "ret\n\t"
  "C_BIN_LEN_ELSE_IF%=:\n\t"
    "cmpb $0, %%al\n\t"
    "jns C_BIN_LEN_END_IF%=\n\t"
    "movl $-1, %%eax\n\t"
    "leave\n\t"
    "ret\n\t"
  "C_BIN_LEN_END_IF%=:\n\t"
    "inc %%ebx\n\t"
    "cmpb $0, %%al\n\t"
    "je DO_WHILE_BIN_LEN%=\n\t"
  "END_DO_WHILE_BIN_LEN%=:\n\t"
    "cmpl $1, %%ebx\n\t"
    "je END_FOR_BIN_LEN%=\n\t"
    "dec %%ebx\n\t"
  "FOR_BIN_LEN%=:\n\t"
    "cmpl $0, %%ebx\n\t"
    "jl END_FOR_BIN_LEN%=\n\t"
    "movl %0, %%ecx\n\t"
    "shll $1, %%ecx\n\t"
    "andl $1, %%eax\n\t"
    "orl %%eax, %%ecx\n\t"
    "movl %%ecx, %0\n\t"
    "cmpl $1, %%ebx\n\t"
    "je END_FOR_BIN_LEN%=\n\t"
    "xorq %%rax, %%rax\n\t"
    "call decompressor\n\t"
    "dec %%ebx\n\t"
    "jmp FOR_BIN_LEN%=\n\t"
  "END_FOR_BIN_LEN%=:\n\t"
    : "=m" (write_num)
    : "m" (write_num)
    : "memory", "%eax", "%ebx", "%ecx", "%edx");
  /* clang-format on */
  fprintf(stderr, "Write num - %d [decoder]\n", write_num);
  fprintf(out_fd, "%u ", write_num);
  /* clang-format off */
    __asm__ volatile (
    "xorl %%ecx, %%ecx\n\t"
    "movl %%ecx, %0\n\t"
    "cmpb %%cl, %%al\n\t"
    "jl DO_WHILE_DECODER_2_END\n\t"
    "jmp DO_WHILE_DECODER_2_START\n\t"
  "DO_WHILE_DECODER_2_END:"
    : "=m" (write_num)
    :
    : "memory");
  /* clang-format on */
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
unsigned char decompressor() {
  static unsigned char decompressor_read_byte = 0;
  static int decompressor_byte_index = -1;
  unsigned char high_bit = 0;
  unsigned char return_code = 0;
  if (decompressor_byte_index < 0) {
    decompressor_byte_index = 7;
    return_code = read(STDIN_FILENO, &decompressor_read_byte, 1);
    if (return_code <= 0)
      return -1;
  }
  high_bit = (decompressor_read_byte >> decompressor_byte_index) & 0x1;
  --decompressor_byte_index;
  return high_bit;
}
