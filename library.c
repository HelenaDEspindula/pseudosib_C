
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"

int compare ( const void *pa, const void *pb ) 
{
	const int *a = *(const int **)pa;
	const int *b = *(const int **)pb;

	return a[ID_COL] - b[ID_COL];
}


void sort_by_column (int** matrix, int max_lin, int max_col)
{
	qsort (matrix, max_lin, sizeof matrix[0], compare);

	printf("ordenei\n");

	print_matrix (matrix, max_lin, max_col);
}


/* -- Input reading -- */

void input_read ()
{
	int i = 0;
	int j = 0;
	char ch_temp;
	int allele1, allele2;

	/* -- Assignments -- */

	num_col_in = FIXED_COL + covariables + 2*markers;
	num_ind = NIND;
	printf("Num_col_fixo = %d\n", num_col_in);

	/* -- Open imput file to read -- */

	file_in = fopen (name_file_in, "r");
	if ( ! file_in )
   {
		error_screen(9);
   }
	printf("Arquivo aberto\n");

	/* -- Malloc rs_markers -- */

	markers_list = malloc_vector_mark ( markers_list, markers); 

	/* -- File reading of rs_markers -- */

	for (i=0; i < markers; i++)
	{
		fscanf (file_in, "%s", markers_list[i].marker_name );
		printf ("rs_markers[ %d ] = %s\n", i, (markers_list[i]).marker_name); // Fazer parte de erro
	}

	/* -- Dynamic allocation of input matrix -- */

	input_matrix = malloc_matrix_int (input_matrix, num_ind, num_col_in); 

	printf("Malloc matriz ponteiro = %p\n", input_matrix);

	/* -- Reading lines of individuals -- */

	fseek (file_in, +1, SEEK_CUR);
	ch_temp = fgetc (file_in);
	if (ch_temp == '\n')
	{
		ch_temp = fgetc (file_in);
	}
	i = 0; // count of lines
	while ( ch_temp != EOF )
	{
		printf("To nesse while\n");
		while ( (i < num_ind) && (ch_temp != EOF) )
		{
			j = 0;
			while (ch_temp != '\n')
			{
				if ( (ch_temp >= '0') && (ch_temp <= '9') )
				{
					fseek (file_in, -1, SEEK_CUR);
					fscanf (file_in, "%d", &input_matrix[i][j]);
					j++;
					ch_temp = fgetc (file_in);
				}
				else if ( ch_temp == ' ' )
				{
					ch_temp = fgetc (file_in);
					if ( ch_temp == ' ' )
					{
						printf ("Error in input file (2 spaces): row = %d, colun = %d", i+2, j+1);
						free_matrix(input_matrix, ZERO, num_ind);
						error_screen(12);
					}
				}
				else if ( ch_temp == '\r' )
				{
					ch_temp = fgetc (file_in);
				}
				else if (ch_temp != '\n')
				{
					printf ("Error in input file (not number char): row = %d, colun = %d", i+2, j+1);
					free_matrix(input_matrix, ZERO, num_ind);
					error_screen(13);
				}
			}
			if ( j != num_col_in )
			{
				printf ("Wrong number of columns in row %d", i+1);
				free_matrix(input_matrix, ZERO, num_ind);
				error_screen(14);
			}
			i++;
			ch_temp = fgetc (file_in);
		}
				
		if ( ch_temp != EOF ) // If file content is larger than the matrix -> do realloc of matrix
		{
			input_matrix = realloc_matrix_int (input_matrix, (num_ind*2) , num_col_in, i);
			num_ind = num_ind*2;
			printf("Realloc matriz ponteiro = %p\n", input_matrix);
		}
	
	}
	num_ind = i;
	printf("I FINISHED TO READ THE ENTRY, with %d individuals\n", num_ind);
}

/* -- Realloc Matrix (int) -- */

int** realloc_matrix_int (int** matrix, int lin , int col, int old_last_lin)
{
	int i =0;
	int **pnt_temp1, **pnt_temp2;

	pnt_temp1 = matrix;
	pnt_temp2 = (int**) realloc (matrix, ( lin * sizeof (int*)) );
	printf("Ponteiro antigo = %p, ponteiro novo = %p \n", pnt_temp1, pnt_temp2);
	printf("Valor ponteiro antigo = %d, valor ponteiro novo = %d \n", **pnt_temp1, **pnt_temp2);
		
	if (pnt_temp2 != NULL)
	{
		printf("Realloc funcionou\n");
		matrix=pnt_temp2;
		printf("fiz\n");
	}
	else
	{
		printf("erro\n");
		free_matrix(pnt_temp1, ZERO, num_ind);
		error_screen(11);
	}
	for (i = old_last_lin; i < lin; i++)
	{
		matrix[i] = malloc_vector_int (matrix[i], col);
	}
	printf("Funcao realloc matriz ponteiro = %p\n", matrix);
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
	printf("Malloc matriz input Ok.\n");

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

	printf("Malloc matriz markers Ok.\n");
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

/* -- Initialization -- */

void initialization (int argc, char **argv)
{
	int initial_error = 0;

	initial_error = comand_line (argc, argv);

	if (initial_error == 0) // If comand line ok
	{
		printf("Comand line ok.\n");
	}
	else
	{
		error_screen(initial_error);
	}
}

/* -- Comand Line Interpretation -- */

int comand_line (int argc, char **argv)
{
	//printf ("Numero de parâmetros: %d\n", argc);

	bool input = false;
	bool output = false;
	bool mark = false;
	bool cov = false;
	char * extension;

	// Default vallues:
	missing = '.';
	separated = ' ';
	
	int i = 1;
	while (i<argc)
	{
		//printf ("argv[%d]: %s\n", i, argv[i]);

		if ( (strcmp ("-h", argv[i]) == 0) || (strcmp ("--help", argv[i]) == 0) )
		{
			return(1); // Help screen
		}
		else if (strcmp ("-i", argv[i]) == 0)
		{
			i++;
			if (argc<=i)
			{
				return(4);
			}
			extension = strrchr (argv[i], '.');
			if (extension == NULL)
			{
				return(3);
			}
			else if (strcmp (".ped", extension) != 0)
			{
				return(3);
			}
			strcpy (name_file_in, argv[i]);
			input = true;
		}
		else if (strcmp ("-o", argv[i]) == 0)
		{
			i++;
			if (argc<=i)
			{
				return(4);
			}
			strcpy (name_file_out, argv[i]);
			output = true;
		}
		else if ( (strcmp ("-c", argv[i]) == 0) || (strcmp ("--covariables", argv[i]) == 0) )
		{
			i++;
			if (argc<=i)
			{
				return(4);
			}
			covariables = atoi (argv[i]);
			cov = true;
		}
		else if ( (strcmp ("-m", argv[i]) == 0) || (strcmp ("--markers", argv[i]) == 0) )
		{
			i++;
			if (argc<=i)
			{
				return(4);
			}
			markers = atoi (argv[i]);
			mark = true;
		}
		else if ( (strcmp ("-n", argv[i]) == 0) || (strcmp ("--notavailable", argv[i]) == 0) )
		{
			i++;
			if (argc<=i)
			{
				return(4);
			}
			if (strcmp ("point", argv[i]) == 0)
			{
				missing = '.';
			}
			else if (strcmp ("na", argv[i]) == 0)
			{
				missing = 'n';
			}
			else
			{
				return(2); // Unknown option vallue
			}
		}
		else if ( (strcmp ("-s", argv[i]) == 0) || (strcmp ("--separated", argv[i]) == 0) )
		{
			i++;
			if (argc<=i)
			{
				return(4);
			}
			if (strcmp ("space", argv[i]) == 0)
			{
				separated = ' ';
			}
			else if (strcmp ("tab", argv[i]) == 0)
			{
				separated = '\t';
			}
			else
			{
				return(2); // Unknown option vallue
			}
		}
		else
		{
			return(4); // Unknown option
		}
		i++;
	}

	if ( (input == false) || (output == false) || (mark == false) || (cov == false) ) 
	// Test if all mandatory are present
	{
		return(5); // Mandatory ausent
	}
	else
	{
		printf ("Input file: %s;\n", name_file_in);
		printf ("Output file: %s;\n", name_file_out);
		printf ("Covariables: %d;\n", covariables);
		printf ("Markers: %d;\n", markers);
		printf ("Not Available: [%c];\n", missing);
		printf ("Separated: [%c].\n", separated);

		return(0); // Initialization ok
	}
}

/* -- Error Scren -- */

void error_screen (int error)
{
	printf("\n");
	printf("Closing program due to error: ");

	if ( (error >= 1) && (error <=5) )
	{
		if (error != 1)
		{
			printf("Problems with the command line. "); // Complementar
			switch (error)
			{
				case 2:
		    		printf("Unknown option vallue.");
					break ;
				case 3:
		    		printf("Incorrect file type.");
					break ;
				case 4:
		    		printf("Unknown option.");
					break ;
				case 5:
		    		printf("Mandatory option ausent.");
					break ;
			}
			printf("\n");
		}
		printf("\n");
		printf("PseudoSib v %f \n", VERSION);
		printf("\n");
		printf("Usage:\n");
		printf("$ ./pseudosib -i <input.ped> -o <output.txt> -c <int> -m <int> [-h] [-n <string>] [-s <string>] \n");
		printf("\n");
		printf("\n");
		printf("Options:\n");
		printf("-h, --help \t \t \t Help screen and software version \n");
		printf("-c, --covariables <int> \t Number of covariables \n");
		printf("-m, --markers <int> \t \t Number of markers \n");
		printf("-n, --notavailable <string> \t 'point' (default) or 'na' \n");
		printf("-s, --separated <string> \t 'space' (default) or 'tab' \n");
		printf("\n");
	}

	switch (error)
		{
			case 6:
       		printf(" ");
				break;
			case 7:
       		printf(" ");
				break;
			case 8:
       		printf(" ");
				break;
			case 9:
       		printf("Error opening file");
				break;
			case 10:
				printf ("Error allocating memory");
				break;
			case 11:
				printf ("Error reallocating memory");
				break;
			case 12:
				printf ("Error in input file");
				break;
			case 13:
				printf ("Error in input file");
				break;
			case 14:
				printf ("Error in input file");
				break;
		}

	printf("\n");

	exit(error);
}

/* -- Print Matrix -- */

void print_matrix (int ** matrix, int max_lin, int max_col)
{
	int i, j;

	for (i=0; i<max_lin; i++)
	{
		for (j=0; j<max_col-1; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("%d\n", matrix[i][max_col-1]);
	}

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

	printf("Fiz free\n");
}

/* -- Header -- */

void header()
{
	printf ("Caractéristiques du fichier de données à respecter pour la création de pseudogermains (pour chromosome X, utiliser un autre programme):\n");
	printf (" -> les colonnes doivent être séparées par des espaces ou des tabulations\n");
	printf (" -> les individus (au maximum 1500) doivent être classés par famille\n");
	printf (" -> les premières colonnes doivent correspondre à: family_id, id, father_id, mother_id, sex, affected status, covariables (maximum 10), marqueurs (allèle1 allèle2)\n");
	printf (" -> les id ne doivent pas comporter de caractères alphanumériques(0 si manquant)\n");
	printf (" -> sex et affected status doivent être codés 1 ou 2 (0 si manquant)\n");
	printf (" -> les allèles (au maximum 100 marqueurs) doivent être recodés 1 ou 2 (0 si manquant, les males sont codés comme des homozygotes)\n");
}
