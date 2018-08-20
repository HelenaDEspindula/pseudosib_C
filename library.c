
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"
#include "liballoc.h"
#include "libinout.h"
#include "global.h"

int compare ( const void *pa, const void *pb ) 
{
	const int *a = *(const int **)pa;
	const int *b = *(const int **)pb;

	return a[ID_COL] - b[ID_COL];
}


void sort_by_column (int** matrix, int max_lin, int max_col)
{
	qsort (matrix, max_lin, sizeof matrix[0], compare);

	//printf("ordenei\n");

	//print_matrix (matrix, max_lin, max_col);
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

		printf ("\t%s: \t major = %d, minor = %d.\n", ((markers_list[m]).marker_name), (markers_list[m]).major_allele, (markers_list[m]).minor_allele);
		m++;
	}
	printf("\n");

}


int find_family ()
{
	trio_list = malloc_vector_trio (trio_list, (num_ind*TRIO_RATE) );

	printf("Counting trios:\n");

	find_trios(input_matrix, num_ind, num_col_in);

	output_vector = malloc_vector_natural_sibs (output_vector, num_trios);

	// find_natural_sibs (output_vector, num_trios);

	return(num_trios*4 + num_solo);
}

void find_natural_sibs (int ** vector, int max_lin)
{
	int i, t;
	int v = 0;
	int lin = max_lin;
	int more_two = FALSE;
	
	for (t=0; t<lin; t++)
	{
		for (i=t+1; i<lin; i++)
		{
			while ( ((trio_list[t]).father == (trio_list[i]).father) && ((trio_list[t]).mather == (trio_list[i]).mather) )
			{
				printf("Achei um irmão natural no trio %d!\n", t);

				if ( (trio_list[t]).child2 == NULL )
				{
					(trio_list[t]).child2 = (trio_list[i]).child1;	

					if (i != lin)
					{
						(trio_list[i]).father = (trio_list[lin]).father;
						(trio_list[i]).mather = (trio_list[lin]).mather;
						(trio_list[i]).child1 = (trio_list[lin]).child1;
						(trio_list[i]).child2 = NULL;

					}

					lin = lin -1;

					
				}
				else if ( (trio_list[t]).child2 != NULL )
				{
					more_two = TRUE;
					(vector[v]) = (trio_list[i]).child1;
					printf("Vector1[%d] = %p\n", v, (vector[v]));
					v = v + 1;

					if (i != lin)
					{
						(trio_list[i]).father = (trio_list[lin]).father;
						(trio_list[i]).mather = (trio_list[lin]).mather;
						(trio_list[i]).child1 = (trio_list[lin]).child1;
						(trio_list[i]).child2 = NULL;
					}

					lin = lin -1;
				}
			

			}
		}

		if (more_two == TRUE)
		{
			(vector[v]) = (trio_list[t]).child1;
			printf("Vector2[%d] = %p\n", v, (vector[v]));
			v = v + 1;
			(vector[v]) = (trio_list[t]).child2;
			printf("Vector3[%d] = %p\n", v, (vector[v]));
			v = v + 1;

			(trio_list[t]).father = (trio_list[lin-1]).father;
			(trio_list[t]).mather = (trio_list[lin-1]).mather;
			(trio_list[t]).child1 = (trio_list[lin-1]).child1;
			(trio_list[t]).child2 = NULL;

			lin = lin -1;
			more_two = FALSE;
		}


	}

	num_trios = lin;
	num_solo = v;
}


void find_trios (int** matrix, int max_lin, int max_col) // Coloca os trios na estrutura
{
	int i, t;
	t = 0;

	//printf("Entrei achar trios\n");

	for(i=0; i < max_lin; i++)
	{
		//printf("Linha %d: ID = %d, FT = %d, MT = %d \n", i, matrix[i][ID_COL], matrix[i][FT_COL], matrix[i][MT_COL]);
		if ( (matrix[i][ST_COL] == 2) && (matrix[i][FT_COL] != 0) && (matrix[i][MT_COL] != 0) ) // Se nenhum dos pais é zero
		{
			
			(trio_list[t]).father = search_id(matrix, max_lin, matrix[i][FT_COL]); 
			(trio_list[t]).mather = search_id(matrix, max_lin, matrix[i][MT_COL]);
			(trio_list[t]).child1 = matrix[i];
			(trio_list[t]).child2 = NULL;
			(trio_list[t]).mendelian_error = 0;

			//printf("Ponteiro f = %p, m = %p\n", (trio_list[t]).father, (trio_list[t]).mather);

			if ( ( (trio_list[t]).father != NULL ) && ( (trio_list[t]).mather != NULL ) ) // Se os pais existem na tabela
			{
				if ( ( ((trio_list[t]).father)[SX_COL] != 1 ) || ( ((trio_list[t]).mather)[SX_COL] != 2 ) )
				{
					// ERRO DE GENERO DOS PAIS
				}
				else
				{
					printf("\tI found the %d trio: c = %d, f = %d, m = %d \n", t + 1, ((trio_list[t]).child1)[ID_COL], ((trio_list[t]).father)[ID_COL], ((trio_list[t]).mather)[ID_COL]);
					t++;
				}
			}
			else // Se os pais não existem
			{
				//printf("  Um ou mais pais não estão na tabela\n");

				// Futuro caso dos irmãos;

				(trio_list[t]).child1 = NULL;
				(trio_list[t]).father = NULL;
				(trio_list[t]).mather = NULL;	
			}

		}
		else
		{
			//printf("  Um ou mais pais zerados\n");
		}
	}
	num_trios = t; // conferir
}



int * search_id (int** matrix, int max_lin, int query_id)
{
	// No momento busca um por um, pode ser otimizado

	//printf("Busca\n");

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


void output_make()
{
	int i, j, t, k, l, s;
	int info;

	//printf("Entrei em output_make.\n");
	printf("\nCounting mendelian erros:\n");

	i = 0;
	t = -1;
	k = input_matrix[num_ind-1][ID_COL] +1; // Numeração IDs irmãos, a partir do ultimo numero de ID + 1;

	if (mark_col == ONE_COL)
	{
		num_col_out = FIXED_COL + covariables + markers;
	}
	else if (mark_col == TWO_COL)
	{
		num_col_out = num_col_in;
	}

	output_matrix = malloc_matrix_int (output_matrix, num_output, num_col_out);

	//printf("i = %d ; num_output = %d ; t = %d ; num_trios = %d. \n", i, num_output, t, num_trios);

	while( (i<num_output) && (t<num_trios) )
	{
		//printf("i = %d, iresto4 = %d.\n", i, (i%4));

		if (i%4 == 0)
		{
			t++;
			//printf("Linha %d = Filho real do trio %d\n", i, t );
			for(j=0; j<FIXED_COL+covariables; j++)
			{
				output_matrix[i][j] = ((trio_list[t]).child1)[j];
			}
			
			if (mark_col == ONE_COL)
			{
				j=FIXED_COL+covariables;
				l=FIXED_COL+covariables;

				while (j<num_col_in)
				{

					if ( ((trio_list[t]).child1)[j] != ((trio_list[t]).child1)[j + 1] )
					{
						output_matrix[i][l] = HETOR;
					}
					else if ( ((trio_list[t]).child1)[j] == (markers_list[l-(FIXED_COL+covariables)]).major_allele )
					{
						output_matrix[i][l] = HOM_MAJ;
					}
					else if ( ((trio_list[t]).child1)[j] == (markers_list[l-(FIXED_COL+covariables)]).minor_allele )
					{
						output_matrix[i][l] = HOM_MIN;
					}

					//printf("J = %d, L = %d, m = %d\n",j, l, output_matrix[i][l]);

					j = j + 2;
					l++;
				}
			}
				
			else if (mark_col == TWO_COL)
			{
				for(j=FIXED_COL+covariables; j<num_col_out; j++)
				{
					output_matrix[i][j] = ((trio_list[t]).child1)[j];
				}
			}		
/*
			for(j=0; j<num_col_out;j++)
			{
				printf("%d ", output_matrix[i][j]);
			}
			printf("\n");
*/
			i++;
		}
		else
		{
			for (s=0; s<NUM_SIBS;s++)
			{
				//printf("Linha %d = Filho virtual do trio %d\n", i, t );
				output_matrix[i+s][FM_COL] = ((trio_list[t]).child1)[FM_COL];
				output_matrix[i+s][ID_COL] = k;
				k++;
				output_matrix[i+s][FT_COL] = ((trio_list[t]).child1)[FT_COL];
				output_matrix[i+s][MT_COL] = ((trio_list[t]).child1)[MT_COL];
				output_matrix[i+s][SX_COL] = ((trio_list[t]).child1)[SX_COL];
				output_matrix[i+s][ST_COL] = NOT_AFT;
				
				for(j=FIXED_COL; j<FIXED_COL+covariables; j++)
				{
					output_matrix[i+s][j] = ((trio_list[t]).child1)[j];
				}
			}
			
			j = FIXED_COL + covariables;
			l = FIXED_COL + covariables;
			while(j<num_col_in) // por coluna
			{
				info = make_sibs( ((trio_list[t]).father)[j], ((trio_list[t]).father)[j+1], ((trio_list[t]).mather)[j], ((trio_list[t]).mather)[j+1], ((trio_list[t]).child1)[j], ((trio_list[t]).child1)[j+1]);
				
				
				//printf("Linha(i) = %d, Coluna(j) = %d, Info = %d \n", i, j, info );

				if (info == 1) // não informativo
				{
					//printf("Info = 1\n");
					if (mark_col == ONE_COL) // Só para testar
					{
						for(s=-1; s<NUM_SIBS; s++)
						{
							output_matrix[i+s][l] = -1;
						}
					}
					else if (mark_col == TWO_COL) // Só para testar
					{
						for(s=-1; s<NUM_SIBS; s++)
						{
							output_matrix[i+s][j] = -1;
							output_matrix[i+s][j+1] = -1;
						}
					}
				}
				else if (info == 2) // sem informação de genotipo
				{
					if (mark_col == ONE_COL) // Só para testar
					{
						for(s=-1; s<NUM_SIBS; s++)
						{
							output_matrix[i+s][l] = -2;
						}
					}
					else if (mark_col == TWO_COL) // Só para testar
					{
						for(s=-1; s<NUM_SIBS; s++)
						{
							output_matrix[i+s][j] = -2;
							output_matrix[i+s][j+1] = -2;
						}
					}

				}
				else if (info == 3) // erro mendeliano
				{
					if (mark_col == ONE_COL) // Só para testar
					{
						for(s=-1; s<NUM_SIBS; s++)
						{
							output_matrix[i+s][l] = -3;
						}
					}
					else if (mark_col == TWO_COL) // Só para testar
					{
						for(s=-1; s<NUM_SIBS; s++)
						{
							output_matrix[i+s][j] = -3;
							output_matrix[i+s][j+1] = -3;
						}
					}
					(trio_list[t]).mendelian_error = (trio_list[t]).mendelian_error + 1;
					printf( "\tTrio = %d, Child = %d, Mendelian error = %d\n", t, (((trio_list[t]).child1)[ID_COL]), (trio_list[t]).mendelian_error);

					if ( (lme_on_off == TRUE) && (trio_list[t]).mendelian_error > max_lme)
					{
						printf( "Essa familia tem erro mendeliano demais = %d\n", (trio_list[t]).mendelian_error );
					}

					
				}
				else if (info == 0)
				{
					//printf("Sibs: %d, %d, %d\n", sibs[0], sibs[1], sibs[2]);
					if (mark_col == ONE_COL) // Só para testar
					{
						for (s=0; s<3; s++)
						{
							if ( (sibs[s] / 10) != (sibs[s] % 10) )
							{
								output_matrix[i+s][l] = HETOR;
							}
							else if ( (sibs[s] / 10) == (markers_list[l-(FIXED_COL+covariables)]).major_allele )
							{
								output_matrix[i+s][l] = HOM_MAJ;
							}
							else if ( (sibs[s] / 10) == (markers_list[l-(FIXED_COL+covariables)]).minor_allele )
							{
								output_matrix[i+s][l] = HOM_MIN;
							}
						}
					}
					else if (mark_col == TWO_COL) // Só para testar
					{
						for (s=0; s<NUM_SIBS; s++)
						{
							output_matrix[i+s][j] = sibs[s] / 10;
							output_matrix[i+s][j+1] = sibs[s] % 10;
						}
					} 
				}
				j = j+2;
				l++;
			}
			i = i+3;
		}
	}

/*
	printf("Imprimindo matriz output.\n");

	print_matrix (output_matrix, num_output, num_col_out);

	printf("Fim da matriz.\n");
*/
}

int make_sibs( int fa1, int fa2, int ma1, int ma2, int ca1, int ca2)
{
	int s1a1, s1a2, s2a1, s2a2, s3a1, s3a2, s4a1, s4a2;

	//printf("Na make sibs.\n");

	//printf("Pai: %d %d, Mae: %d %d, Filho: %d %d\n", fa1, fa2, ma1, ma2, ca1, ca2);

	if ( (fa1 == 0) || (fa2 == 0) || (ma1 == 0) || (ma2 == 0) || (ca1 == 0) || (ca2 == 0) ) 
	{
		return(2); // sem informação de genotipo
	}

	s1a1 = fa1;
	s1a2 = ma1;

	s2a1 = fa1;
	s2a2 = ma2;

	s3a1 = fa2;
	s3a2 = ma1;

	s4a1 = fa2;
	s4a2 = ma2;

	if ( (fa1 != fa2) && (ma1 != ma2)) // Pais heterozigotos
	{
		if ( ((ca1 == s1a1) && (ca2 == s1a2)) || ((ca2 == s1a1) && (ca1 == s1a2)) )
		{
			sibs[0] = s2a1 * 10 + s2a2;
			sibs[1] = s3a1 * 10 + s3a2;
			sibs[2] = s4a1 * 10 + s4a2;
		}
		else if ( ((ca1 == s2a1) && (ca2 == s2a2)) || ((ca2 == s2a1) && (ca1 == s2a2)) )
		{
			sibs[0] = s1a1 * 10 + s1a2;
			sibs[1] = s3a1 * 10 + s3a2;
			sibs[2] = s4a1 * 10 + s4a2;
		}
		else if ( ((ca1 == s3a1) && (ca2 == s3a2)) || ((ca2 == s3a1) && (ca1 == s3a2)) )
		{
			sibs[0] = s2a1 * 10 + s2a2;
			sibs[1] = s1a1 * 10 + s1a2;
			sibs[2] = s4a1 * 10 + s4a2;
		}
		else if ( ((ca1 == s4a1) && (ca2 == s4a2)) || ((ca2 == s4a1) && (ca1 == s4a2)) )
		{
			sibs[0] = s2a1 * 10 + s2a2;
			sibs[1] = s1a1 * 10 + s1a2;
			sibs[2] = s3a1 * 10 + s3a2;
		}
		else
		{
			//printf("Erro mend 1\n");
			return(3); // erro mendeliano
		}
		return(0);

	}
	else if ( (fa1 == fa2) && (ma1 == ma2) ) //homo
	{
		if (ca1 == ca2)
		{
			//printf("Erro mend 2\n");
			return(3); // erro mendeliano
		}
		else
		{
		// if (fa1 != ma1) // homo diferentes
		// if (fa1 == ma1) // homo iguais 
		// De qualquer forma não é informativo
		return(1); // não informativo, zera todos os filhos
		}
	}
	else // Um pai heterozigoto
	{
		if ( ((ca1 == fa1) && (ca2 == fa2)) || ((ca2 == fa2) && (ca1 == fa1)) )
		{
			sibs[0] = ma1 *10 + ma2;
			sibs[1] = 0;
			sibs[2] = 0;
		}
		else if ( ((ca1 == ma1) && (ca2 == ma2)) || ((ca2 == ma2) && (ca1 == ma1)) )
		{
			sibs[0] = fa1 *10 + fa2;
			sibs[1] = 0;
			sibs[2] = 0;
		}
		else
		{
			//printf("Erro mend 3\n");
			return(3); // erro mendeliano
		}
		return(0);
	}
}

