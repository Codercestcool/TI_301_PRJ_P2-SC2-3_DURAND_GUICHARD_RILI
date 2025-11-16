#include <stdio.h>
#include <stdlib.h>
#include "hasse2.h"

/* Création d'un tableau dynamique pour stocker les liens*/
t_link_array *createLinkArray(int capacite_initiale) {
    t_link_array *arr = malloc(sizeof(t_link_array));
    arr->links = malloc(sizeof(t_link) * capacite_initiale);
    arr->taille = 0;
    arr->capacite = capacite_initiale;
    return arr;
}

/* Vérifie si un lien existe déjà (évite les doublons) */
int linkExists(t_link_array *arr, int depart, int arrivee) {
    for (int i = 0; i < arr->taille; i++) {
        if (arr->links[i].depart == depart && arr->links[i].arrivee == arrivee)
            return 1;
    }
    return 0;
}

/* Ajoute un lien au tableau si non présent*/
void addLink(t_link_array *arr, int depart, int arrivee) {
    if (linkExists(arr, depart, arrivee)) return; // évite doublon

    if (arr->taille == arr->capacite) {
        arr->capacite *= 2;
        arr->links = realloc(arr->links, sizeof(t_link) * arr->capacite);
    }
    arr->links[arr->taille].depart = depart;
    arr->links[arr->taille].arrivee = arrivee;
    arr->taille++;
}

/* Contruit un tableau sommet → classe*/
int *buildSommetToClasse(t_classe_list *classe_list, int nb_sommets) {
    int *sommetToClasse = malloc(sizeof(int) * nb_sommets);

    for (int c = 0; c < classe_list->nb_classes; c++) {
        for (int k = 0; k < classe_list->classes[c].taille; k++) {
            int sommet = classe_list->classes[c].sommets[k];
            sommetToClasse[sommet] = c; // associe sommet à sa classe
        }
    }
    return sommetToClasse;
}

/*   Construit les liens entre classes selon les transitions du graphe */
void construireLiensHasse(
    int **adj, int *adjSize, int nb_sommets,
    t_classe_list *classes, t_link_array *links
) {
    int *sommetToClasse = buildSommetToClasse(classes, nb_sommets);

    for (int i = 0; i < nb_sommets; i++) {
        int Ci = sommetToClasse[i]; // classe du sommet i

        for (int k = 0; k < adjSize[i]; k++) {
            int j = adj[i][k];
            int Cj = sommetToClasse[j]; // classe du sommet j

            if (Ci != Cj) {
                addLink(links, Ci, Cj);
            }
        }
    }
    free(sommetToClasse);
}

/* Affiche le diagramme de Hasse au format Mermaid */
void afficherHasseMermaid(t_link_array *links) {
    printf("graph TD\n");
    for (int i = 0; i < links->taille; i++) {
        printf("  C%d --> C%d\n",
            links->links[i].depart + 1,
            links->links[i].arrivee + 1
        );
    }
}
