
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

void allels_def (int** matrix, int max_lin, int max_col, int last_col)
{
	int i, j, k, m;
	int freq1, freq2, freq3, freq4;
	m = 0;
	j = (max_col - (last_col*2));


	while(j < max_col)
	{
		freq1 = 0;
		freq2 = 0;
		freq3 = 0;
		freq4 = 0;
		for(k=0; k < 2; k++)
		{
			for(i=0; i < max_lin; i++)
			{
			//printf ("matriz[%d][%d] = rs_markers[%d] = %d\n", i, j, m, matrix[i][j]);	
				if(matrix[i][j] == 1)
				{
					freq1++;
				}
				else if(matrix[i][j] == 2)
				{
					freq2++;
				}
				else if(matrix[i][j] == 3)
				{
					freq3++;
				}
				else if(matrix[i][j] == 4)
				{
					freq4++;
				}
				else if(matrix[i][j] != 0)
				{
					error_screen (6);
				}
			}
			j++;
		}


		// ARRUMAR !!!!!!!!!!!!!!!!!!!!!!!!!!!1
		//if ( (freq1 != 0) && (freq2 != 0) && (freq3 != 0) ) || ( (freq1 != 0) && (freq2 != 0) && (freq4 != 0) ) || ( (freq2 != 0) && (freq3 != 0) && (freq4 != 0) ) || ( (freq1 != 0) && (freq3 != 0) && (freq4 != 0) )
		//{
		//	error_screen (7);
		//}

		if( (freq1>=freq2) && (freq1>=freq3) && (freq1>=freq4)) // 1 is major
		{
			(markers_list[m]).major_allele = 1;
			if (freq2 != 0)
			{
				(markers_list[m]).minor_allele = 2;
			}
			else if (freq3 != 0)
			{
				(markers_list[m]).minor_allele = 3;
			}
			else if (freq4 != 0)
			{
				(markers_list[m]).minor_allele = 4;
			}
		}
		else if( (freq2>=freq1) && (freq2>=freq3) && (freq2>=freq4)) // 2 is major
		{
			(markers_list[m]).major_allele = 2;
			if (freq1 != 0)
			{
				(markers_list[m]).minor_allele = 1;
			}
			else if (freq3 != 0)
			{
				(markers_list[m]).minor_allele = 3;
			}
			else if (freq4 != 0)
			{
				(markers_list[m]).minor_allele = 4;
			}
		}
		else if( (freq3>=freq2) && (freq3>=freq1) && (freq3>=freq4)) // 3 is major
		{
			(markers_list[m]).major_allele = 3;
			if (freq2 != 0)
			{
				(markers_list[m]).minor_allele = 2;
			}
			else if (freq1 != 0)
			{
				(markers_list[m]).minor_allele = 1;
			}
			else if (freq4 != 0)
			{
				(markers_list[m]).minor_allele = 4;
			}
		}
		else if( (freq4>=freq2) && (freq4>=freq1) && (freq4>=freq3)) // 4 is major
		{
			(markers_list[m]).major_allele = 4;
			if (freq2 != 0)
			{
				(markers_list[m]).minor_allele = 2;
			}
			else if (freq3 != 0)
			{
				(markers_list[m]).minor_allele = 3;
			}
			else if (freq1 != 0)
			{
				(markers_list[m]).minor_allele = 1;
			}
		}

		printf ("rs_markers[ %d ]: major: %d, minor %d.\n", m, (markers_list[m]).major_allele, (markers_list[m]).minor_allele);
		m++;
	}

}


int find_trios (int** matrix, int max_lin, int max_col)
{
	int i, t;
	t = 0;

	printf("Entrei achar trios\n");

	trio_list = malloc_vector_trio (trio_list, (max_lin*TRIO_RATE) );

	for(i=0; i < max_lin; i++)
	{
		printf("Linha %d: ID = %d, FT = %d, MT = %d \n", i, matrix[i][ID_COL], matrix[i][FT_COL], matrix[i][MT_COL]);
		if ( (matrix[i][ST_COL] == 2) && (matrix[i][FT_COL] != 0) && (matrix[i][MT_COL] != 0) ) // Se nenhum dos pais é zero
		{

			(trio_list[t]).child = matrix[i];
			(trio_list[t]).father = search_id(matrix, max_lin, matrix[i][FT_COL]); // FAZER FUNÇÃO !!!!
			(trio_list[t]).mather = search_id(matrix, max_lin, matrix[i][MT_COL]);

			printf("Ponteiro f = %p, m = %p\n", (trio_list[t]).father, (trio_list[t]).mather);

			if ( ( (trio_list[t]).father != NULL ) && ( (trio_list[t]).mather != NULL ) ) // Se os pais existem na tabela
			{
				if ( ( ((trio_list[t]).father)[SX_COL] != 1 ) || ( ((trio_list[t]).mather)[SX_COL] != 2 ) )
				{
					// ERRO DE GENERO DOS PAIS
				}
				else
				{
					printf("Achei o %d trio:\n c = %d, f = %d, m = %d \n", t, ((trio_list[t]).child)[ID_COL], ((trio_list[t]).father)[ID_COL], ((trio_list[t]).mather)[ID_COL]);
					t++;
				}
			}
			else // Se os pais não existem
			{
				printf("  Um ou mais pais não estão na tabela\n");

				// Futuro caso dos irmãos;

				(trio_list[t]).child = NULL;
				(trio_list[t]).father = NULL;
				(trio_list[t]).mather = NULL;	
			}

		}
		else
		{
			printf("  Um ou mais pais zerados\n");
		}
	}
	num_trios = t-1;

	return(num_trios*4);
}



int * search_id (int** matrix, int max_lin, int query_id)
{
	// No momento busca um por um, pode ser otimizado

	printf("Busca\n");

	int test = 0;

	while ( matrix[test][ID_COL] < query_id )
	{
		//printf("Procurando %d, na linha %d achei %d \n", query_id, test, matrix[test][ID_COL] );
		test++;
	}
	if ( matrix[test][ID_COL] == query_id )
	{
		return(matrix[test]);
	}
	else
	{
		return(NULL);
	}

}


/* -- Input reading -- */

void input_read ()
{
	int i = 0;
	int j = 0;
	char ch_temp;

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

	markers_list = malloc_vector_mark (markers_list, markers); 

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

	printf("Malloc estruct list markers Ok.\n");
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

	printf("Malloc estruct list trio Ok.\n");
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
	mark_col = ONE_COL;
	
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
		else if ( (strcmp ("-g", argv[i]) == 0) || (strcmp ("--genotypes", argv[i]) == 0) )
		{
			i++;
			if (argc<=i)
			{
				return(4);
			}
			if (strcmp ("1", argv[i]) == 0)
			{
				mark_col = 1;
			}
			else if (strcmp ("2", argv[i]) == 0)
			{
				mark_col = 2;
			}
			else
			{
				return(2);
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
		printf ("Genotypes in [%d] columns.\n", mark_col);

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
		printf("$ ./pseudosib -i <input.ped> -o <output.txt> -c <int> -m <int> [-h] [-n <string>] [-s <string>] [-g <int>]\n");
		printf("\n");
		printf("\n");
		printf("Options:\n");
		printf("-h, --help \t \t \t Help screen and software version \n");
		printf("-c, --covariables <int> \t Number of covariables \n");
		printf("-m, --markers <int> \t \t Number of markers \n");
		printf("-n, --notavailable <string> \t 'point' (default) or 'na' \n");
		printf("-s, --separated <string> \t 'space' (default) or 'tab' \n");
		printf("-g, --genotypes <int> \t '1' (default) or '2', number of columns used for genotypes in output.\n");
		printf("\n");
	}

	switch (error)
		{
			case 6:
       		printf("Error in the coding of allels. Only numbers 1 to 4 allowed.");
				break;
			case 7:
       		printf("Error in the coding of allels. Only binary markers allowed.");
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

	free_all();

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

	printf("Fiz free matriz\n");
}


void output_make()
{
	int i, j, t, k;
	//int fa1, fa2, ma1, ma2, ca1, ca2;
	//int s1a1, s1a2, s2a1, s2a2, s3a1, s3a2, s4a1, s4a2;

	printf("Entrei em output_make.\n");

	i = 0;
	t = -1;
	k = input_matrix[num_ind][ID_COL] +1; // Numeração IDs irmãos

	if (mark_col == ONE_COL)
	{
		num_col_out = FIXED_COL + covariables + markers;
	}
	else if (mark_col == TWO_COL)
	{
		num_col_out = num_col_in;
	}

	output_matrix = malloc_matrix_int (output_matrix, num_output, num_col_out);

	while( (i<num_output) && (t<num_trios) )
	{
		//printf("i = %d, iresto4 = %d.\n", i, (i%4));

		if (i%4 == 0)
		{
			t++;
			printf("Linha %d = Filho real do trio %d\n", i, t );
			for(j=0; j<covariables; j++)
			{
				output_matrix[i][j] = ((trio_list[t]).child)[j];
			}
			if (mark_col == ONE_COL)
			{
				for(j=covariables; j<num_col_out; j++) // Só para testar
				{
					output_matrix[i][j] = 0; // A fazer
				}
			}
			else if (mark_col == TWO_COL)
			{
				for(j=covariables; j<num_col_out; j++)
				{
					output_matrix[i][j] = ((trio_list[t]).child)[j];
				}
			}

			i++;
		}
		else
		{
			printf("Linha %d = Filho virtual do trio %d\n", i, t );
			output_matrix[i][FM_COL] = ((trio_list[t]).child)[FM_COL];
			output_matrix[i][ID_COL] = k;
			k++;
			output_matrix[i][FT_COL] = ((trio_list[t]).child)[FT_COL];
			output_matrix[i][MT_COL] = ((trio_list[t]).child)[MT_COL];
			output_matrix[i][SX_COL] = ((trio_list[t]).child)[SX_COL];
			output_matrix[i][ST_COL] = NOT_AFT;
			
			for(j=FIXED_COL; j<covariables; j++)
			{
				output_matrix[i][j] = ((trio_list[t]).child)[j];
			}
			
			if (mark_col == ONE_COL) // Só para testar
			{
				for(j=covariables; j<num_col_out; j++) // Só para testar
				{
					output_matrix[i][j] = 0; // A fazer
				}
			}
			else if (mark_col == TWO_COL)
			{
				for(j=covariables; j<num_col_out; j++) // Só para testar
				{
					output_matrix[i][j] = 1; // A fazer
				}
			}

/*
			if (i%3 == 0)
			{
				for(j=covariables; j<num_col_in; j+2) // por coluna
				{
					fa1 = ((trio_list[t]).father)[j];
					fa2 = ((trio_list[t]).father)[j+1];
					ma1 = ((trio_list[t]).mather)[j];
					ma2 = ((trio_list[t]).mather)[j+1];
					ca1 = ((trio_list[t]).child)[j];
					ca2 = ((trio_list[t]).child)[j+1];
					
					if ( (fa1 != fa2) && (ma1 != ma2))
					{
						

					}
					else if ( (fa1 == fa2) && (ma1 == ma2) ) && (fa1 == ma1) )
					{
						//homo 
						if (fa1 == ma1)
						{
							// identicos
						}
						else
						{
							// não identicos
						}
						
					}
					else if 
					{
						// heteroz
					}



				}
			
			}
*/

			i++;
		}
	}

	printf("Imprimindo matriz output.\n");

	print_matrix (output_matrix, num_output, num_col_out);
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
