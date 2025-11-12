#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "tarjan.h"
#include "pile.h"

// PARTIE 2

t_classe_list *createClasseList(int capacite_initiale) {
    t_classe_list *list = malloc(sizeof(t_classe_list));
    list->classes = malloc(capacite_initiale * sizeof(t_classe));
    list->nb_classes = 0;
    list->capacite = capacite_initiale;
    return list;
}

void addClasse(t_classe_list *list, int *sommets, int taille) {
    if (list->nb_classes == list->capacite) {
        list->capacite *= 2;
        list->classes = realloc(list->classes, list->capacite * sizeof(t_classe));
    }

    t_classe *c = &list->classes[list->nb_classes++];
    c->sommets = malloc(taille * sizeof(int));
    for (int i = 0; i < taille; i++)
        c->sommets[i] = sommets[i];
    c->taille = taille;
}

t_tarjan_sommet * tab_sommets(t_list_adj graph){
    int n = graph.taille;
    t_tarjan_sommet * sommets = malloc(n * sizeof(t_tarjan_sommet));
    if (sommets == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++){
        sommets[i].id = i;
        sommets[i].num = -1;
        sommets[i].num_acc = -1;
        sommets[i].booleen = 0;
    }
    return sommets;
}

void parcours(t_list_adj *graph, t_tarjan_sommet *sommets, int v, t_classe_list *partition) {
    sommets[v].num = num;
    sommets[v].num_acc = num;
    num++;

    empiler(&pile, v);
    sommets[v].booleen = 1;

    t_cell *current = graph->liste[v].head;
    while (current != NULL) {
        int w = current->arrivee;

        if (sommets[w].num == -1) {
            parcours(graph, sommets, w, partition);
            if (sommets[w].num_acc < sommets[v].num_acc)
                sommets[v].num_acc = sommets[w].num_acc;
        } else if (sommets[w].booleen) {
            if (sommets[w].num < sommets[v].num_acc)
                sommets[v].num_acc = sommets[w].num;
        }

        current = current->suivant;
    }

    if (sommets[v].num_acc == sommets[v].num) {
        // Racine d'une composante
        int temp[MAX_SOMMETS];
        int count = 0;
        int w;
        do {
            w = depiler(&pile);
            sommets[w].booleen = 0;
            temp[count++] = w;
        } while (w != v);

        addClasse(partition, temp, count);
    }
}


// Fonction Tarjan
t_classe_list *tarjan(t_list_adj *graph) {
    num = 0;
    initPile(&pile);
    t_tarjan_sommet *sommets = tab_sommets(*graph);

    t_classe_list *partition = createClasseList(4);

    for (int i = 0; i < graph->taille; i++) {
        if (sommets[i].num == -1)
            parcours(graph, sommets, i, partition);
    }

    free(sommets);
    libererPile(&pile);
    return partition;
}