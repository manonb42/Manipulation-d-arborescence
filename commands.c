#include "commands.h"
#include "arbre.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


// Ajoute un fichier (ou dossier) dans le parent.
bool ajoute_fichier(noeud *pere, noeud *fichier){
    if (!pere->est_dossier || liste_noeud_trouver(pere->fils, fichier->nom) != NULL){
        return false;
    }
    noeud_ajoute_fils(pere, fichier);
    return true;
}

// Creer un fichier ou dossier
bool creer_fichier(noeud *wd, char* chemin, bool est_dossier){
    char *nom = chemin_to_nom(wd, chemin);
    noeud *pere = chemin_to_parent(wd, chemin);

    noeud *n = new_noeud(est_dossier, nom, wd->racine);
    if (n == NULL) return false;
    if(!ajoute_fichier(pere, n)){
        return false;
    }
    free(nom);
    return true;
}


noeud *ls(noeud *wd, int argc, char **argv) {
    if (argc != 1) return NULL;

    if (argc == 1){ 
        for (liste_noeud *act = wd->fils; act != NULL; act = act->succ) {
			printf("%s\n", act->no->nom);
		}
        return wd;
    }
    return NULL;
}

noeud *cd(noeud *wd, int argc, char **argv) {
    if (argc == 1) {
        return wd->racine;
    }
    if(argc == 2){
        if (strcmp(argv[1], "..") == 0) {
            return wd->pere;
        }
        noeud *res = chemin_to_noeud(wd, argv[1]);
        if (res == NULL || !res->est_dossier) {
            printf("cd : Destination invalide\n");
            return NULL;
        } else{ return res; }
    }
    return NULL;
}

noeud *pwd(noeud *wd, int argc, char **argv) {
    if (argc != 1) return NULL;

    int size = 0;
    for (noeud *cur=wd; cur->pere != cur; cur = cur->pere)
        size++;
    noeud **noeuds = calloc(size, sizeof(noeud*));
    int i = size;
    for (noeud *cur=wd; cur->pere != cur; cur = cur->pere)
        noeuds[--i] = cur;

    if (size == 0)
        printf("/");
    else for (int i=0; i<size; ++i)
        printf("/%s", noeuds[i]->nom);
    printf("\n");
    free(noeuds);
    return wd;   
}

noeud *mkdir(noeud *wd, int argc, char **argv) {
    if (argc != 2) return NULL;

    if (creer_fichier(wd, argv[1], true)){
        return wd;
    } else {
        printf("mkdir : Destination invalide\n");
        return NULL;
    }
}

noeud *touch(noeud *wd, int argc, char **argv) {
    if (argc != 2) return NULL;

   if (creer_fichier(wd, argv[1], false)){
        return wd;
    } else {
        printf("touch : Destination invalide\n");
        return NULL;
    }
}

noeud *rm(noeud *wd, int argc, char **argv) {
    if (argc != 2) return NULL;

    noeud *cible = chemin_to_noeud(wd, argv[1]);
    if(cible == NULL || est_descendant(cible, wd)){
        printf("rm : Destination invalide\n");
        return NULL;
    } 
    noeud_enleve_fils(cible->pere, cible->nom);
    free_noeud(cible);
    return wd;
}

noeud *cp(noeud *wd, int argc, char **argv) {
    if(argc!=3) return NULL;
    
    noeud *src = chemin_to_noeud(wd, argv[1]);
    noeud *pere = chemin_to_parent(wd, argv[2]);
    if (src == NULL || pere == NULL || est_descendant(src, pere)){
        printf("cp : Destination invalide\n");
        return NULL;
    }

    noeud *copie = copie_noeud(src);
    char *nom = chemin_to_nom(wd, argv[2]); 
    if (!renomme_noeud(copie, nom)) return NULL;
    free(nom);

    ajoute_fichier(pere, copie);
    return wd;    
}

noeud *mv(noeud *wd, int argc, char **argv) {
    if(argc!=3) return NULL;

    noeud *src = chemin_to_noeud(wd, argv[1]);
	noeud *pere = chemin_to_parent(wd,argv[2]);
    if (src == NULL || pere == NULL || est_descendant(src, pere)) {
        printf("mv : Destination invalide\n");
		return NULL;
	}

    char *nom = chemin_to_nom(wd, argv[2]);
    noeud_enleve_fils(src->pere, src->nom);
    if (!renomme_noeud(src, nom)) return NULL;
    free(nom);
    ajoute_fichier(pere, src);
    return wd;
}

void print_rec(noeud *wd) {
    if (wd == NULL) {
        return;
    }
    if (wd->est_dossier) {
        printf("REPERTORY : ");
    } else {
        printf("FILE : ");
    }
    if ((wd->nom)[0] == '\0') {
        printf(" ROOT");
    }
    printf("%s | Father : %s | Children :", wd->nom, wd->pere->nom);
    liste_noeud *tmp = wd->fils;
    if (tmp == NULL) {
        printf(" None");
    }
    while (tmp != NULL) {
        printf(" %s  ", tmp->no->nom);
        tmp = tmp->succ;
    }
    printf("\n");
    tmp = wd->fils;
    while (tmp != NULL) {
        print_rec(tmp->no);
        tmp = tmp->succ;
    }
}

noeud *print(noeud *wd, int argc, char **argv) {
    if (argc != 1) return NULL;

    print_rec(wd->racine);
    puts("");
    return wd;
}

noeud *exec_command(noeud *wd, int argc, char **argv){
	if(argc == 0) return NULL;
    if (strcmp("ls", argv[0]) == 0) return ls(wd, argc, argv);
    else if (strcmp("cd", argv[0]) == 0) return cd(wd, argc, argv);
    else if (strcmp("pwd", argv[0]) == 0) return pwd(wd, argc, argv);
    else if (strcmp("mkdir", argv[0]) == 0) return mkdir(wd, argc, argv);
    else if (strcmp("touch", argv[0]) == 0) return touch(wd, argc, argv);
    else if (strcmp("rm", argv[0]) == 0) return rm(wd, argc, argv);
    else if (strcmp("cp", argv[0]) == 0) return cp(wd, argc, argv);
    else if (strcmp("mv", argv[0]) == 0) return mv(wd, argc, argv);
    else if (strcmp("print", argv[0]) == 0) return print(wd, argc, argv);
    else return NULL;
}
