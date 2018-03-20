						 /* PSEUDO-SIBLINGS CREATION PROGRAM */
							/* by Helena R S D'Espindula */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"
#include "liballoc.h"
#include "libinout.h"
#include "global.h"

 
int main (int argc, char **argv, char **envp)
{
	initialization (argc, argv);

	input_read (); // Leitura

	allels_def(input_matrix, num_ind, num_col_in, markers);	// Alelos Mais e Menos frequentes &

	sort_by_column (input_matrix, num_ind, num_col_in);	// Ordenação input_matrix

	num_output = find_family ();	

	output_make();

	free_all();

	// Print output_matrix in file

	return (0);
}
