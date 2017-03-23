
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"


void header()
{
/*titre*/
  printf ("Caractéristiques du fichier de données à respecter pour la création de pseudogermains (pour chromosome X, utiliser un autre programme):\n");
  printf (" -> les colonnes doivent être séparées par des espaces ou des tabulations\n");
  printf (" -> les individus (au maximum 1500) doivent être classés par famille\n");
  printf (" -> les premières colonnes doivent correspondre à: family_id, id, father_id, mother_id, sex, affected status, covariables (maximum 10), marqueurs (allèle1 allèle2)\n");
  printf (" -> les id ne doivent pas comporter de caractères alphanumériques(0 si manquant)\n");
  printf (" -> sex et affected status doivent être codés 1 ou 2 (0 si manquant)\n");
  printf (" -> les allèles (au maximum 100 marqueurs) doivent être recodés 1 ou 2 (0 si manquant, les males sont codés comme des homozygotes)\n");
}
