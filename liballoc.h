
#ifndef __LIBALLOC__
#define __LIBALLOC__
#include "global.h"

/* Allocating functions */

int** realloc_matrix_int (int** matrix, int lin , int col, int old_last_lin);

int** malloc_matrix_int (int** input_matrix, int num_ind, int num_col_in);

Markers_t * malloc_vector_mark (Markers_t * vector, int col);

Trio_t * malloc_vector_trio (Trio_t * vector, int col);

int ** malloc_vector_natural_sibs (int ** vector, int lin);

int* malloc_vector_int (int* vector, int col);

void free_all();

void free_matrix(int ** matrix, int lin_ini, int lin_fin);

#endif