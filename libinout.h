#ifndef __LIBINOUT__
#define __LIBINOUT__
#include <stdio.h>
#include "global.h"

void input_read ();

void initialization (int argc, char **argv);

int comand_line (int argc, char **argv);

void error_screen (int error);

void print_matrix (int ** matrix, int max_lin, int max_col);

#endif