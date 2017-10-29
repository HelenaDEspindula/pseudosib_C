						 /* PSEUDO-SIBLINGS CREATION PROGRAM */
							/* by Helena R S D'Espindula */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"

 
int main (int argc, char **argv, char **envp)
{
	initialization (argc, argv);

	input_read (); // Leitura

	allels_def(input_matrix, num_ind, num_col_in, markers);	// Alelos Mais e Menos frequentes &

	sort_by_column (input_matrix, num_ind, num_col_in);	// Ordenação input_matrix

	num_output = find_trios (input_matrix, num_ind, num_col_in);	// Coloca os trios na estrutura

	output_make();

	free_all();

	// Pseudo-sib (intermediary_matrix -> output_matrix)

	// Print output_matrix in file

	return (0);
}
