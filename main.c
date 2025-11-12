#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main() {
    t_list *liste = createEmptyList();
    addCell(liste, 1, 0.3);
    addCell(liste, 2, 0.7);
    displayList(liste); // on affiche la liste pour la partie 1 (cette partie sera retirée après)
    printf("\n\n");

    printf("Création d'une liste d'adjacence vide de taille 3 :\n");
    t_list_adj adj = createEmptyAdjList(3);

    addCell(&(adj.tab[0]), 1, 0.5);
    addCell(&(adj.tab[0]), 2, 0.5);
    addCell(&(adj.tab[1]), 0, 1.0);
    addCell(&(adj.tab[2]), 2, 1.0);

    printf("Affichage de la liste d'adjacence :\n");
    displayListAdj(&adj);

    verifyMarkovGraph(&adj);

    exportToMermaid(&adj, "graph.mmd");

    t_list_adj g2 = readGraph("graph.txt");
    displayListAdj(&g2);
    verifyMarkovGraph(&g2);

    return 0;
}