#ifndef MIP_CODER_H
#define MIP_CODER_H
/*Include Headers-------------------------------------------------------------*/

/*Include user-defined headers------------------------------------------------*/
#include <MIP_helper.h>

/*Constants-------------------------------------------------------------------*/

/*Declarations----------------------------------------------------------------*/

/*Functions-------------------------------------------------------------------*/
int coder(int code);
int fi0_coder();
int fi1_coder();
int fi2_coder();
int get_binary_length(int num);
int compressor(char sym, int out_fd);
/*----------------------------------------------------------------------------*/
#endif
