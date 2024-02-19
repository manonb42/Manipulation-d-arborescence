#include "arbre.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//Noeud


noeud *init_arbre() {
    noeud *racine = malloc(sizeof(noeud));
    assert(racine != NULL);
    racine->est_dossier = 1;
    racine->nom[0] = '\0';
    racine->pere = racine;
    racine->fils = NULL;
    racine->racine = racine;
    return racine;
}


int que_alpha(char * s) {
	for(unsigned i = 0; i < strlen(s); ++i) {
		if(!isalpha(s[i]) && !isdigit(s[i]) && s[i] != '/') return false;
	}
	return true;
}

bool renomme_noeud(noeud *n, char *nom){
    if(nom[0] == 0 || strlen(nom) > 99 || !que_alpha(nom)){
        printf("Le nom  de fichier '%s' est invalide \n", nom);
        return false;
    }
    strncpy(n->nom, nom, 100);
    return true;
}

noeud* new_noeud(bool est_dossier, char *nom, noeud *racine){
    noeud *n = calloc(1, sizeof(noeud));
    n->est_dossier = est_dossier;
    n->racine = racine;
    if (!renomme_noeud(n, nom)) {free(n); return NULL;}
    return n;
}

void free_noeud(noeud *n){
	if(n == NULL) return;
    free_liste_noeud(n->fils);
    free(n);
}

void free_liste_noeud(liste_noeud *liste){
	if(liste == NULL) return;
    liste_noeud *next = liste->succ;
    for (liste_noeud *cur = liste; cur != NULL; cur = next){
        next = cur->succ;
        free_noeud(cur->no);
        free(cur);
    }
}

bool est_descendant(noeud *pere, noeud *fils){
    if(pere==fils) return true;
    for (noeud *cur=fils; cur->pere!=cur; cur=cur->pere)
        if (cur==pere) return true;
    return false;
}

void noeud_ajoute_fils(noeud *pere, noeud *fils){
    fils->pere = pere;
    pere->fils = liste_noeud_ajouter(pere->fils, fils);
}

void noeud_enleve_fils(noeud *pere, char *nom){
    pere->fils = liste_noeud_enlever(pere->fils, nom);
}

noeud *copie_noeud(noeud *n){
    noeud *res = malloc(sizeof(noeud));
    memcpy(res, n, sizeof(noeud));
    res->fils = new_liste_noeud();
    for (liste_noeud *cur=n->fils; cur!=NULL; cur=cur->succ)
        noeud_ajoute_fils(res, copie_noeud(cur->no));
    return res;
}




liste_noeud* new_liste_noeud(){
    return NULL;
}

int liste_noeud_taille(liste_noeud* l){
    int i = 0;
    for (liste_noeud *cur = l; cur != NULL; cur = cur->succ)
        ++i;
    return i;
}

liste_noeud *liste_noeud_ajouter(liste_noeud* l, noeud* n){
    liste_noeud *el = calloc(1, sizeof(liste_noeud));
    el->no = n;
	el->succ = l;
	return el;
}

liste_noeud *liste_noeud_enlever(liste_noeud *l, char *n) {
	if(l == NULL) return NULL;
    if (strcmp(l->no->nom, n) == 0) {
		liste_noeud *res = l->succ;
		free(l);
		return res;
	}
    for (liste_noeud *cur=l; cur->succ!=NULL; cur=cur->succ){
        if (strcmp(cur->succ->no->nom, n)==0) {
            liste_noeud *tmp = cur->succ;
            cur->succ = cur->succ->succ;
            free(tmp);
            return l;
        }
    }
    return l;
}

noeud *liste_noeud_trouver(liste_noeud *l, char* nom){
    for (liste_noeud *cur = l; cur != NULL; cur=cur->succ)
        if (strcmp(cur->no->nom, nom) == 0) return cur->no;
    return NULL;
}




//Chemins

char **split_string(char *chemin, char* separateur){
    char *chem = strdup(chemin);
    int size = 1;
    for(int i=0; i<strlen(chem); ++i)
        if(chem[i]==separateur[0]) ++size;
    char** composantes = calloc(size+1, sizeof(char*));
    int i=0;
    for(char* mot = strtok(chem, separateur); mot!= NULL; mot = strtok(NULL, separateur)){
        composantes[i]=strdup(mot);
        i++;
    }
    free(chem);
    return composantes;
}

char** composantes_chemin(char *chemin){
    return split_string(chemin, "/");
}

noeud *composantes_to_noeud(noeud *wd, char **composantes){
    noeud *res = wd;
    for (char **cur = composantes; *cur != NULL; ++cur){
        noeud *next = liste_noeud_trouver(res->fils, *cur);
        if (next == NULL) {
            return NULL;
		}
        res = next;
	}
    return res;
}

noeud *chemin_to_noeud(noeud *wd, char *chemin) {
    if (chemin[0] == '/') {
        wd = wd->racine;
        ++chemin;
    }
    char **composantes = composantes_chemin(chemin);
    noeud *res = composantes_to_noeud(wd, composantes);

    for (char **cur = composantes; *cur != NULL; ++cur) free(*cur);
    free(composantes);
    return res;
}

noeud *chemin_to_parent(noeud *wd, char *chemin) {

    if (chemin[0] == '/') {
        wd = wd->racine;
        ++chemin;
    }

    char **composantes = composantes_chemin(chemin);
    
    int dernier ;
    for (dernier = 0; composantes[dernier+1] != NULL; ++dernier);
    free(composantes[dernier]);
    composantes[dernier] = NULL;

    noeud *res = composantes_to_noeud(wd, composantes);

    for (char **cur = composantes; *cur != NULL; ++cur) free(*cur);
    free(composantes);
    return res;
}

char *chemin_to_nom(noeud *wd, char *chemin) {
    char **composantes = composantes_chemin(chemin);
  
    int dernier;
    for (dernier = 0; composantes[dernier+1] != NULL; ++dernier);

    char *res = composantes[dernier];

    for (int i=0; i<dernier; ++i) free(composantes[i]);
    free(composantes);
    return res;
}
