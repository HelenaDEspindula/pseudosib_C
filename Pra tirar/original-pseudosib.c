                             /* PROGRAMME DE CREATION DE PSEUDOGERMAINS */


# define NIND 1500  /*nombre max d'individus*/
# define NBM 100  /* nombre max de marqueurs*/
# include <stdio.h>
 
main ()

{ int geno [NIND][2*NBM + 6+10];
  int pseudo [NIND*4][2*NBM+6+10];
  int real [NIND][2*NBM+6+10];
  int type [NIND];
  int i=0,j,k,h,l,m, n, trio, geno_pere,geno_mere,geno_aff,geno_sain,sex_aff, sib_tdt=0,tdt=0; 
  int nind=0, nbm=0, incomp=0, fam=0, cov, multi, aff, sain, nreal,nmulti;
  char ficgeno[81], ficrec[81]; 
  FILE * entree, * sortie;

/*titre*/
  printf ("Caract�ristiques du fichier de donn�es � respecter pour la cr�ation de pseudogermains (pour chromosome X, utiliser un autre programme):\n");
  printf ("  les colonnes doivent �tre s�par�es par des espaces ou des tabulations\n");
  printf ("  les individus (au maximum 1500) doivent �tre class�s par famille\n");
  printf ("  les premi�res colonnes doivent correspondre �: family_id, id, father_id, mother_id, sex, affected status, covariables (maximum 10), marqueurs (all�le1 all�le2)\n");
  printf ("  les id ne doivent pas comporter de caract�res alphanum�riques(0 si manquant)\n");
  printf ("  sex et affected status doivent �tre cod�s 1 ou 2 (0 si manquant)\n");
  printf ("  les all�les (au maximum 100 marqueurs) doivent �tre recod�s 1 ou 2 (0 si manquant, les males sont cod�s comme des homozygotes)\n");

/*lecture du fichier de donn�es et des param�tres de base*/
  printf ("\n Donner le nom du fichier � analyser: \n");
  scanf ("%80s", ficgeno);

  printf ("donner le nombre de marqueurs (au maximum 100)\n");
  scanf ("%d", &nbm);

  printf ("donner le nombre de covariables (au maximum 10)\n");
  scanf ("%d", &cov);

  entree = fopen (ficgeno, "r");
  while (1)
  	 { fscanf (entree, "%d", &geno[i][0]); 
  	   if (feof (entree)) break;
  	   for (j=1; j<(2*nbm + 6 + cov); j=j+1) fscanf (entree, "%d", &geno[i][j]);
  	   i=i+1;
  	   nind=i;
  	 }
  fclose (entree);
  printf ("%d individus\n", nind);

 /* lecture de la structure et des g�notypes des famille pour rep�rage des familles de type sib_tdt */

  for (i=0; i<nind; i=i+1) if (geno [i][0]!=fam)
  { /*1*/
   fam=geno[i][0];
	
   geno_pere=0;
   geno_mere=0;
   geno_aff=0;
   geno_sain=0;
   aff=0;
   sain=0;
   
   for (k=0; geno[i+k][0]==fam; k=k+1)
     { /*2 v�rification du g�notypage des parents et enfants*/
     if ((geno[i+k][3]==0) && (geno [i+k][4]==1)) 
       for (j=1; j<2*nbm; j=j+2) geno_pere=geno_pere+geno[i+k][6+cov+j];
     if ((geno[i+k][3]==0) && (geno [i+k][4]==2)) 
       for (j=1; j<2*nbm; j=j+2) geno_mere=geno_mere+geno[i+k][6+cov+j];
     /* comptage des enfants atteints g�notyp�s*/
     if ((geno[i+k][3]!=0) && (geno [i+k][5]==2)) 
       {
       for (j=1; j<2*nbm; j=j+2) geno_aff=geno_aff + geno[i+k][6+cov+j];
       if (geno_aff != 0) aff=aff + 1;
       }
     /* comptage des enfants sains g�notyp�s*/
     if ((geno[i+k][3]!=0) && (geno [i+k][5]==1)) 
       {
       for (j=1; j<2*nbm; j=j+2) geno_sain=geno_sain + geno[i+k][6+cov+j];
       if (geno_sain != 0) sain=sain + 1;
       }
     } /*2*/
   
    /* si pas d'enfant atteint: famille �limin�e*/
   if (aff==0)  incomp=incomp+1; 
 /* si les 2 parents sont g�notyp�s : tdt*/
   else if ((geno_mere!=0) && (geno_pere!=0) ) 
     { /*2*/
      type[i] = 1; 
      tdt=tdt+1;
      } /*2*/  
   /* si au moins 1 parent manque enti�rement et au moins un enfant sain: sib_tdt*/          
   else if (sain >0) 
     { /*2*/
      type[i] = 2; 
      sib_tdt=sib_tdt+1;
      } /*2*/  
   
    /* si un des parents manque enti�rement et pas d'enfant sain: famille �limin�e*/           
   else incomp=incomp+1; 
    
  } /*1*/

/* marqueur par marqueur (2 alleles)*/
 for (j=0 ; j<nbm*2 ;j=j+2)
 { /*1*/
  trio=0;
  multi=0;
  nmulti=0;
  nreal=0;
  
/* famille par famille*/

  for (i=0; i<nind; i=i+1) if (geno [i][0]!=fam)

  { /*2*/
   fam=geno[i][0];
   aff=0;
   
   if (type[i] ==1)
    {/*3 si trio avec 2 parents g�notyp�s */
    geno_pere=9;
    geno_mere=9;
    geno_aff=9;

    for (k=0; geno[i+k][0]==fam; k=k+1)
     { /*4 lecture de la structure et des g�notypes de la famille */
     if ((geno[i+k][3]==0) && (geno [i+k][4]==1)) geno_pere=geno[i+k][6+cov+j]+geno[i+k][7+cov+j];
     if ((geno[i+k][3]==0) && (geno [i+k][4]==2)) geno_mere=geno[i+k][6+cov+j]+geno[i+k][7+cov+j];
     } /*4*/
   
    for (k=0; geno[i+k][0]==fam; k=k+1)  
     if ((geno[i+k][3]!=0) && (geno [i+k][5]==2))
     { /*4 NB: si plusieurs enfants atteints, duplication des parents (cr�ation d'un nouveau trio)*/
	 aff=aff+1;	 /*aff>1 si plusieurs enfants atteints dans la m�me famille*/

	 geno_aff=geno[i+k][6+cov+j]+geno[i+k][7+cov+j];	 
     sex_aff= geno [i+k][4];
  
     /* attribution d'un num�ro de cluster et d'un num�ro d'individu � l'int�rieur du cluster */
  	 for (l=0;l<4;l=l+1) pseudo[trio*4+l][0]= fam;  
 	  pseudo[trio*4][1]= geno[i+k][1];
     for (l=1;l<4;l=l+1) pseudo[trio*4+l][1]= (aff-1)*3 + l;  
  	 
     /*renum�rotation des parents en 1 et 2*/
  	 for (l=0;l<4;l=l+1) pseudo[trio*4+l][2]= 0;
  	 for (l=0;l<4;l=l+1) pseudo[trio*4+l][3]= 0; 
    
     /*covariables des cas et controle*/
 	 for (m=4;m<6+cov;m=m+1) 
     for(l=0;l<4;l=l+1) pseudo[trio*4+l][m]= geno[i+k][m];
          
    /*affected status des cas et du controle*/
     pseudo[trio*4][5]= 1;
     for(l=1;l<4;l=l+1) pseudo[trio*4+l][5]= 0;
	  
	 if ((geno_aff==0 ) || (geno_mere==0) || (geno_pere==0))  /*on invalide tout le trio si mere ou enfant est 00*/
        for (l=0;l<4;l=l+1) pseudo[trio*4+l][6+cov+j]= 1;
  
      /*cr�ation des pseudo controles si 2 parents h�t�rozygotes*/
 	 else if ((geno_pere==3) && (geno_mere==3)) 
      { /*5*/
 	   if (geno_aff==2)
 	    {
  	    pseudo[trio*4][6+cov+j]= 2;
  	    pseudo[trio*4+1][6+cov+j]= 4;
  	    pseudo[trio*4+2][6+cov+j]= 3;
  	    pseudo[trio*4+3][6+cov+j]= 3;
   	    }
   	   if (geno_aff==3)
   	    {
   	    pseudo[trio*4][6+cov+j]= 3;
   	    pseudo[trio*4+1][6+cov+j]= 3;
   	    pseudo[trio*4+2][6+cov+j]= 2;
   	    pseudo[trio*4+3][6+cov+j]= 4;
   	    }
   	   if (geno_aff==4)
  	    {
  	    pseudo[trio*4][6+cov+j]= 4;
  	    pseudo[trio*4+1][6+cov+j]= 2;
  	    pseudo[trio*4+2][6+cov+j]= 3;
  	    pseudo[trio*4+3][6+cov+j]= 3;
  	    }
      } /*5*/
      /*cr�ation des pseudo controles si 1 parent homozygote*/
  	 else if (((geno_pere==2)&&(geno_mere==3)) || ((geno_pere==3)&&(geno_mere==2)))
      { /*5*/
  	  if (geno_aff==2)
  	    {
   	    pseudo[trio*4][6+cov+j]= 2;
  	    pseudo[trio*4+1][6+cov+j]= 3;
  	    pseudo[trio*4+2][6+cov+j]= -1;
   	    pseudo[trio*4+3][6+cov+j]= -1;
   	    }
   	  if (geno_aff==3)
        {
            pseudo[trio*4][6+cov+j]= 3;
   	    pseudo[trio*4+1][6+cov+j]= 2;
   	    pseudo[trio*4+2][6+cov+j]= -1;
   	    pseudo[trio*4+3][6+cov+j]= -1;
   	    }
      } /*5*/
     else if (((geno_pere==4)&&(geno_mere==3)) || ((geno_pere==3)&&(geno_mere==4)))
      { /*5*/
      if (geno_aff==4)
  	    {
  	    pseudo[trio*4][6+cov+j]= 4;
  	    pseudo[trio*4+1][6+cov+j]= 3;
  	    pseudo[trio*4+2][6+cov+j]= -1;
  	    pseudo[trio*4+3][6+cov+j]= -1;
   	    }
      if (geno_aff==3)
  	    {
  	    pseudo[trio*4][6+cov+j]= 3;
  	    pseudo[trio*4+1][6+cov+j]= 4;
  	    pseudo[trio*4+2][6+cov+j]= -1;
  	    pseudo[trio*4+3][6+cov+j]= -1;
   	    }
      } /*5*/
   
       /*cr�ation des pseudo controles si 2 parents homozygotes*/
     else if (((geno_pere==4)||(geno_pere==2)) && ((geno_mere==2)||(geno_mere==4)))
        for (l=0;l<4;l=l+1) pseudo[trio*4+l][6+cov+j]= 1;
   
       trio=trio+1;
       if (aff>1) nmulti=nmulti+1;
     }/*4*/       
         /* comptage des familles multiplex*/	
    
    if (aff>1) {multi=multi+1;nmulti=nmulti+1;}
    } /*3 si trio complet*/
    
   
   else if (type[i]== 2) 
    {/*3 si sib_tdt*/ 
    n=0;
    for (k=0; geno[i+k][0]==fam; k=k+1)  if (geno[i+k][3]!=0) 
     {/*4*/         
           /* num�ro de cluster et num�ro d'individu � l'int�rieur du cluster */ 	  
      real[nreal][0]= fam;  
      real[nreal][1]= geno[i+k][1];  
  	  real[nreal][2]= 0;
  	  real[nreal][3]= 0; 
    
          /*covariables et afs des cas et controles*/
 	  for (m=4;m<6+cov;m=m+1) 
       real[nreal][m]=geno[i+k][m];
  
          /*affected status des cas et controles*/
      if (geno[i+k][5]==2)real[nreal][5]=1;
	else if (geno[i+k][5]==1)real[nreal][5]=0;
	else real[nreal][5]=-1;
  	
         /*genotype des cas et controle = g�notype r��l*/
      if (geno[i+k][6+cov+j]==0)real[nreal][6+cov+j]= -1;
      else real[nreal][6+cov+j]=geno[i+k][6+cov+j]+geno[i+k][7+cov+j];
      
      nreal=nreal+1;
      n=n+1;
     }/*4*/
    }/*3*/
    
  } /*2 famille par famille*/
 } /*1 marqueur par marqueur*/
 
 /* �criture du fichier de sortie*/
 
 printf ("pour %d familles :%d groupes de 1 cas et 3 pseudo-controles ont �t� cr��s pour %d covariable(s) et %d marqueurs\n",tdt,trio,cov,nbm); 
 printf ("dont %d famille multiplex et %d cas ind�pendants\n", multi,trio-nmulti); 
 printf ("%d fratrie(s) avec au moins un parent manquant et un enfant sain (%d individus): les controles sont les enfants sains \n",sib_tdt,nreal); 
 printf ("NB: %d famille(s) incompl�te(s) (pas d'enfant atteint ou un parent non g�notyp� sans enfant sain)ignor�e(s) \n",incomp); 
 printf ("Donner le nom du fichier de sortie: \n");
 scanf ("%80s", ficrec);
 sortie = fopen (ficrec, "w");

 for (i=0; i<trio*4; i=i+1)
 {
   for (j=0; j<(6+cov); j=j+1)
 fprintf (sortie, "%d\t", pseudo[i][j]);
   for (j=6+cov; j<(6+cov+nbm*2); j=j+2)
 fprintf (sortie, "%d\t", pseudo[i][j]);
   fprintf (sortie,"\n");
 }

 for (i=0; i<nreal; i=i+1)
 {
   for (j=0; j<(6+cov); j=j+1)
 fprintf (sortie, "%d\t", real[i][j]);
   for (j=6+cov; j<(6+cov+nbm*2); j=j+2)
 fprintf (sortie, "%d\t", real[i][j]);
   fprintf (sortie,"\n");
 }

 fclose (sortie);

 printf ("Les variables report�es dans %s sont, dans l'ordre:\n",ficrec);
 printf (" famille,individu, pere, mere, sexe, affected status, covariable x %d, g�notype au marqueur x %d\n",cov,nbm);
 printf ("attention : en cas d'erreur mend�lienne, les g�notypes sont cod�s 0 pour tout le cluster");
}

