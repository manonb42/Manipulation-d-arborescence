#ifndef ARBRE_H
#define ARBRE_H
#include <stdbool.h>

struct noeud;
struct liste_noeud;

struct noeud {
    bool est_dossier;
    char nom[100];
    struct noeud *pere;
    struct noeud *racine;
    struct liste_noeud *fils;
};

typedef struct noeud noeud;

struct liste_noeud {
    struct noeud *no;
    struct liste_noeud *succ;
};

typedef struct liste_noeud liste_noeud;

// Initialise un nouvel arbre
noeud *init_arbre();

// Creer un nouveau noeud, retourne NULL en cas d'erreur.
noeud* new_noeud(bool est_dossier, char *nom, noeud* racine);

// Renomme un noeud, retourne vrai en cas de succès.
bool renomme_noeud(noeud* n, char* nom);

// libere la mémoire pour le noeud
void free_noeud(noeud* noeud);

// Libere la liste et tous les noeuds qu'elle contient
void free_liste_noeud(liste_noeud *liste);

// Retourne vrai si fils est un descendant de pere ou si fils == pere,
// Retourne faux sinon. 
bool est_descendant(noeud *pere, noeud *fils);

// Ajoute fils en tant que descendant direct de pere
void noeud_ajoute_fils(noeud *pere, noeud *fils);

//Enleve le noeud nom des fils de pere
void noeud_enleve_fils(noeud *pere, char *nom);

// Retourne une copie profonde du noeud
noeud *copie_noeud(noeud *n);

// Retourne une liste vide (NULL)
liste_noeud* new_liste_noeud();

// Retourne la taille de la liste
int liste_noeud_taille(liste_noeud* l);

// renvoie nouvelle liste avec le noeud ajouté au début
liste_noeud *liste_noeud_ajouter(liste_noeud* l, noeud* n);

// Retourne la liste sans le noeud avec le nom n. Ne fais rien s'il n'est pas dedans.
// L'appelant est responsable de libérer le noeud potentiellement supprimé.
liste_noeud *liste_noeud_enlever(liste_noeud *l, char *n);

// Retourne le noeud dans la liste avec le nom `nom`, où NULL
noeud *liste_noeud_trouver(liste_noeud *l, char* nom);

// Sépare str avec le délimitateur sep, dans un tableau de nouvelles chaines de caractere
char** split_string(char *str, char *sep);

// Retourne chacune des composantes du chemin
// dans un tableau finissant par un NULL. le tableau
// et chacun des elements doivent être libéré. le chemin
// n'est pas modifié.
// (se sert de split_string)
char** composantes_chemin(char *chemin);

// Retourne le noeud indiqué par le tableau de composantes, NULL si il n'existe pas
noeud *composantes_to_noeud(noeud *wd, char **composantes);

// Retourne le noeud indiqué par le chemin, NULL si il n'existe pas
noeud *chemin_to_noeud(noeud *wd, char *chemin);

// Retourne le noeud parent indiqué par le chemin, NULL si il n'existe pas
noeud *chemin_to_parent(noeud *wd, char *chemin);

// Alloue une chaine de charactère contenant le nom du fichier 
// désigné par chemin (sa derniere composante)
char *chemin_to_nom(noeud *wd, char *chemin);

#endif
