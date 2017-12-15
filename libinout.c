
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libinout.h"
#include "library.h"
#include "liballoc.h"
#include "global.h"

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
		//printf ("rs_markers[ %d ] = %s\n", i, (markers_list[i]).marker_name); // Fazer parte de erro
	}

	/* -- Dynamic allocation of input matrix -- */

	input_matrix = malloc_matrix_int (input_matrix, num_ind, num_col_in); 

	//printf("Malloc matriz ponteiro = %p\n", input_matrix);

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
		//printf("To nesse while\n");
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
			//printf("Realloc matriz ponteiro = %p\n", input_matrix);
		}
	
	}
	num_ind = i;
	printf("I FINISHED TO READ THE ENTRY, with %d individuals\n", num_ind);
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
	limit_mend_e = MEND_E;
	
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
		printf("-g, --genotypes <int> \t \t '1' (default) or '2', number of columns used for genotypes in output.\n");
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

