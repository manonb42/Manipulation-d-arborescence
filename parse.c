#include "parse.h"
#include "arbre.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define TAILLE_MAX 1024

// une seule méthode dans ce fichier : celle pour parser 

// prend un FILE* et renvoie un tableau de chaines de caractères 
// est appelé dans un while dans main
char **read_command(FILE *file){

	 
	char *ligne =malloc(TAILLE_MAX);

	// si la ligne est NULL on retourne NULL 
	if (fgets(ligne, TAILLE_MAX, file) == NULL) {
		free(ligne);
		return NULL;
	}
	
	// defini la fin de la chaine
	ligne[strcspn(ligne, "\r\n")] = 0;

	// un print 
	printf("> %s\n", ligne);

	// separe la ligne en tableau avec les différents elem. 
	char **args = split_string(ligne, " ");
	free(ligne);
	return args;
}
