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

//OPTION 1
/* Suppression des liens transitifs (option Hasse strict)
 
   Exemple :
       C1 -> C2
       C2 -> C3
       C1 -> C3   (redondant, sera supprimé)
  */
int pathExists(t_link_array *links, int start, int end) {
    // Recherche récursive de chemin (DFS)
    for (int i = 0; i < links->taille; i++) {
        if (links->links[i].depart == start) {
            int next = links->links[i].arrivee;
            if (next == end) return 1;
            if (pathExists(links, next, end)) return 1;
        }
    }
    return 0;
}

void removeTransitiveLinks(t_link_array *links, int nb_classes) {
    int keep[links->taille]; 
    for (int i = 0; i < links->taille; i++) keep[i] = 1;

    for (int i = 0; i < links->taille; i++) {
        int A = links->links[i].depart;
        int B = links->links[i].arrivee;

        // On regarde s'il existe A -> X -> B
        for (int j = 0; j < links->taille; j++) {
            if (i == j) continue;
            int A2 = links->links[j].depart;
            int M  = links->links[j].arrivee;

            if (A == A2 && M != B) {
                // Si A -> M -> ... -> B alors A -> B est redondant
                if (pathExists(links, M, B)) {
                    keep[i] = 0;
                    break;
                }
            }
        }
    }

    // Construction d'un nouveau tableau de liens filtré
    int newSize = 0;
    for (int i = 0; i < links->taille; i++)
        if (keep[i]) links->links[newSize++] = links->links[i];

    links->taille = newSize;
}
//OPTION 2

/* Suppression des liens transitifs dans p_link_array
  Exemple avant nettoyage :
       C1 -> C2
       C2 -> C3
       C1 -> C3   (redondant)

   Exemple après nettoyage :
       C1 -> C2
       C2 -> C3
   */

static int pathExistsFrom(const t_link_array *arr, int start, int end) {
    for (int i = 0; i < arr->taille; i++) {
        if (arr->links[i].depart == start) {
            int mid = arr->links[i].arrivee;
            if (mid == end) return 1;
            if (pathExistsFrom(arr, mid, end)) return 1;
        }
    }
    return 0;
}

void removeTransitiveLinks(t_link_array *p_link_array) {
    int taille = p_link_array->taille;
    if (taille < 2) return; // rien à nettoyer

    int *keep = malloc(sizeof(int) * taille);
    for (int i = 0; i < taille; i++) keep[i] = 1;

    for (int i = 0; i < taille; i++) {
        int A = p_link_array->links[i].depart;
        int B = p_link_array->links[i].arrivee;

        for (int j = 0; j < taille; j++) {
            if (i == j) continue;
            if (!keep[i]) break;

            int A2 = p_link_array->links[j].depart;
            int M  = p_link_array->links[j].arrivee;

            // On cherche A -> M -> ... -> B
            if (A == A2 && M != B) {
                if (pathExistsFrom(p_link_array, M, B)) {
                    keep[i] = 0; // lien redondant
                }
            }
        }
    }

    // Reconstruction filtrée du tableau de liens
    int newSize = 0;
    for (int i = 0; i < taille; i++) {
        if (keep[i]) {
            p_link_array->links[newSize++] = p_link_array->links[i];
        }
    }
    p_link_array->taille = newSize;
    free(keep);
}
