# Simulateur de Manipulation d'Arborescence de Fichiers/Dossiers
_Manon Baha et Ilian Crague_

## Compilation, execution

```shell
$ make               # Compile le projet
$ ./main fichier.txt # Lance le projet
```

## Description :
Ce projet a pour objectif de développer un programme simulant les instructions système de manipulation de l'arborescence des dossiers et des fichiers. Le programme prendra en entrée un fichier contenant une liste d'instructions et devra simuler ces instructions en manipulant une structure arborescente stockée en mémoire. Les fonctionnalités principales incluent la navigation dans l'arborescence, la création de dossiers/fichiers, le déplacement, la copie ou la suppression des dossiers/fichiers.

## Fonctionnalités prévues :
- Navigation dans l'Arborescence : Le programme permet de se déplacer dans la structure arborescente, d'accéder à différents niveaux de dossiers et de fichiers.
- Création de Dossiers/Fichiers : Il est possible de créer de nouveaux dossiers et fichiers à des emplacements spécifiés dans l'arborescence.
- Déplacement : Le programme autorise le déplacement de fichiers et de dossiers vers d'autres emplacements dans l'arborescence.
- Copie : Il est possible de copier des fichiers et des dossiers, conservant ainsi une copie à l'emplacement d'origine et en créant une nouvelle à l'emplacement spécifié.
- Suppression : Le programme permet la suppression de fichiers et de dossiers de l'arborescence, avec une confirmation facultative pour éviter les suppressions accidentelles.

## État
Les fuites de mémoires sont bien gérées.

