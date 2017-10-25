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

	sort_by_column (input_matrix, num_ind, num_col_in);	// Ordenação input_matrix

	//	Alelos Mais e Menos frequentes &
	// Erros na logica do input

	// Malloc matrix intermediaria

	// Divisão de trios (input_matrix -> intermediary_matrix)

	free_matrix(input_matrix, ZERO, num_ind);

	// Pseudo-sib (intermediary_matrix -> output_matrix)

	// Print output_matrix in file

	return (0);
}
