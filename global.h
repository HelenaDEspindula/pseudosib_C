#ifndef __GLOBAL__
#define __GLOBAL__
#include <stdio.h>
#define NAME_TAM 200
#define VERSION 1.1
#define NIND 2001
#define FM_COL 0
#define ID_COL 1
#define FT_COL 2
#define MT_COL 3
#define SX_COL 4
#define ST_COL 5
#define FIXED_COL 6
#define NOT_AFT 1
#define ZERO 0
#define TRIO_RATE 1.25 // 
#define MEND_E 4
#define TWO_COL 2
#define ONE_COL 1
#define HOM_MAJ 2
#define HOM_MIN 4
#define HETOR 3
#define NUM_SIBS 3
#define FALSE 0
#define TRUE 1
#define NUMBER 0
#define PERCENT 1


/* -- Global variables -- */

int ** input_matrix; // input matrix
int ** output_matrix; // output matrix
int ** output_vector;
int covariables; // real number of covariates
char name_file_in[NAME_TAM]; // name of the input file
char name_file_out[NAME_TAM]; // name of the output file
char error_complement[NAME_TAM];
FILE * file_in;
FILE * file_out;
char missing;
char separated;
int num_solo;
int num_col_in;
int num_col_out;
int num_ind;
int num_trios;
int num_output;
int markers;
char mark_col;
int sibs[NUM_SIBS];
int limit_mend_e;
char lme_on_off;
char lme_num_perc;
float lme_valor;
int max_lme;
int valor_nao_info;
int valor_erro_mend;
int valor_sem_genot;

/*
typedef struct ID_t
{
	char id_name [NAME_TAM] ;
	int *father;
} ID_t;

*/


typedef struct Markers_t
{
  char marker_name [NAME_TAM] ;
  int minor_allele;
  int major_allele;
} Markers_t;

Markers_t *markers_list;

typedef struct Trio_t
{
  int *father; // poiter to father line
  int *mather; // poiter to mather line
  int *child1; // pointer to child1 line
  int *child2;
  int mendelian_error;
} Trio_t;

Trio_t *trio_list;

/* -- End of global variables -- */


#endif