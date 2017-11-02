
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"
#include "liballoc.h"
#include "libinout.h"
#include "global.h"


/* -- Realloc Matrix (int) -- */

int** realloc_matrix_int (int** matrix, int lin , int col, int old_last_lin)
{
	int i =0;
	int **pnt_temp1, **pnt_temp2;

	pnt_temp1 = matrix;
	pnt_temp2 = (int**) realloc (matrix, ( lin * sizeof (int*)) );
	//printf("Ponteiro antigo = %p, ponteiro novo = %p \n", pnt_temp1, pnt_temp2);
	//printf("Valor ponteiro antigo = %d, valor ponteiro novo = %d \n", **pnt_temp1, **pnt_temp2);
		
	if (pnt_temp2 != NULL)
	{
		//printf("Realloc funcionou\n");
		matrix=pnt_temp2;
		//printf("fiz\n");
	}
	else
	{
		//printf("erro\n");
		free_matrix(pnt_temp1, ZERO, num_ind);
		error_screen(11);
	}
	for (i = old_last_lin; i < lin; i++)
	{
		matrix[i] = malloc_vector_int (matrix[i], col);
	}
	//printf("Funcao realloc matriz ponteiro = %p\n", matrix);
	return(matrix);
}

/* -- Malloc Matrix (int) -- */

int** malloc_matrix_int (int** matrix, int lin , int col)
{
	int i =0;

	matrix = (int**) malloc (lin * sizeof (int*)) ;
	if ( matrix == NULL ) // Allocation test
	{
		error_screen(10);
	}

	for (i = 0; i < lin; i++)
	{
		matrix[i] = malloc_vector_int (matrix[i], col);
	}
	//printf("Malloc matriz input Ok.\n");

	return(matrix);
}

/* -- Malloc Vector (Markers_t) -- */

Markers_t * malloc_vector_mark (Markers_t * vector, int col)
{
	vector = (Markers_t *) malloc ( col * sizeof (Markers_t)) ;
	if ( vector == NULL )
	{
		error_screen(10);
	}

	//printf("Malloc estruct list markers Ok.\n");
	return(vector);
}

/* -- Malloc Vector (int) -- */

int* malloc_vector_int (int* vector, int col)
{
	vector = (int*) malloc ( col * sizeof (int)) ;
	if ( vector == NULL )
	{
		error_screen(10);
	}

	return(vector);
}

/* -- Malloc Vector (Trio_t) -- */

Trio_t * malloc_vector_trio (Trio_t * vector, int col)
{
	vector = (Trio_t *) malloc ( col * sizeof (Trio_t)) ;
	if ( vector == NULL )
	{
		error_screen(10);
	}

	//printf("Malloc estruct list trio Ok.\n");
	return(vector);
}

/* -- Malloc Vector (Natural Sibs) -- */

int ** malloc_vector_natural_sibs (int ** vector, int lin)
{
	vector = (int**) malloc (lin * sizeof (int*)) ;
	if ( vector == NULL )
	{
		error_screen(10);
	}

	printf("Malloc natural sibs Ok.\n");
	return(vector);
}

void free_all()
{
	if (input_matrix != NULL)
	{
		free_matrix(input_matrix, ZERO, num_ind);
	}
	if (markers_list != NULL)
	{
		free(markers_list);
		markers_list = NULL;
	}
	if (trio_list != NULL)
	{
		free(trio_list);
		trio_list = NULL;
	}
	if (output_matrix != NULL)
	{
		free_matrix(output_matrix, ZERO, num_output);
	}
	printf("Fiz free de tudo.\n");
}

/* -- Free Matrix -- */

void free_matrix(int ** matrix, int lin_ini, int lin_fin)
{
	int i;

	for (i=lin_ini; i<lin_fin; i++)
	{
		free( matrix[i] );
		matrix[i] = NULL;
	}
	free( matrix );
	matrix = NULL;

	//printf("Fiz free matriz\n");
}