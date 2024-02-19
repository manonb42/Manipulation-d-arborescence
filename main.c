#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "arbre.h"
#include "parse.h"
#include "commands.h"

int main(int argc, char * argv[]) {
	FILE* file=NULL;

	// si on precise pas de fichier, lecture terminal 
	if (argc == 1) file = stdin;

	// sinon, on ouvre en lecture 
	else if (argc == 2) file = fopen(argv[1], "r");
	assert(file != NULL);

	// on initialise la racine de l'arbre 
	noeud * arbre = init_arbre();

	noeud * tmp = arbre;
	char **cmd;

	// tant que la ligne n'est pas vide
	while ((cmd = read_command(file)) != NULL){
		int argc = 0;

		// nombre d'arguments 
		for (char **cur = cmd; *cur != NULL; ++cur) ++argc;

		// tmp = le resultat de la commande 
		tmp = exec_command(arbre, argc, cmd);

		// on free la liste des composantes 
		for (char **cur = cmd; *cur != NULL; ++cur) free(*cur);
		free(cmd);


		if (tmp == NULL) {
			printf("La commande a échoué.\n");
			free_noeud(arbre->racine);
			fclose(file);
			return EXIT_FAILURE;
		}

		// on actualise le working directory 
		arbre = tmp;
	}
	free_noeud(arbre->racine);
	fclose(file);
}
