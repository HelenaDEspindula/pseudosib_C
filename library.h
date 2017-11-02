
#ifndef __LIBRARY__
#define __LIBRARY__
#include <stdio.h>
#include "global.h"




/* --- Functions --- */

int compare ( const void *pa, const void *pb );

void sort_by_column (int** matrix, int max_lin, int max_col);

void allels_def (int** matrix, int max_lin, int max_col, int last_col);

int find_family ();

void find_natural_sibs (int ** vector, int max_lin);

void find_trios (int** matrix, int max_lin, int max_col);

int * search_id (int** matrix, int max_lin, int query_id);

int make_sibs( int fa1, int fa2, int ma1, int ma2, int ca1, int ca2);

void output_make();

#endif
