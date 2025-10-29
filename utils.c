#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static char *getID(int i){
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

// Crée une nouvelle cellule 
t_cell * createCell(int arrivee, float proba) {
    t_cell *cell = (t_cell *)malloc(sizeof(t_cell));
    cell->destination = destination;
    cell->proba = proba;
    return cell;
}

// Crée une liste vide
t_list * createEmptyList() {
    t_list *list = (t_list *)malloc(sizeof(t_list));
    list->head = NULL;
    return list;
}

// Ajoute une cellule au début d’une liste
void addCell(t_list *list, int arrivee, float proba) {
    cell *newCell = createCell(destination, proba);
    if (newCell == NULL) return; // Si l’allocation a échoué, on arrête ici

    // Insertion en tête de liste
    newCell->next = list->head;
    list->head = newCell;
}

// Affiche le contenu d’une liste
void displayList(t_list *list) {
    t_cell *current = list->head;
    printf("[head @] -> ");
    while (cell->suivant != NULL) {
        printf("(%d, %.2f) -> ", cell->destination, cell->proba);
        cell = cell->suivant;
    }
    printf("(%d, %.2f)", cell->destination, cell->proba);
}

// Crée une liste d’adjacence vide pour un nombre donné de sommets
t_list_adj createEmptyAdjList(int size) {
    t_list_adj list_adj;
    list_adj.size = size;
    list_adj.tab = (list *)malloc(size * sizeof(list));

    if (list_adj.tab == NULL) {
        printf("Erreur : allocation mémoire du tableau de listes.\n");
        list_adj.size = 0;
        return G;
    }

    // On initialise chaque liste du tableau comme vide
    for (int i = 0; i < size; i++) {
        list_adj.tab[i] = createEmptyList();
    }

    return list_adj;
}
 
// Lecture du fichier et création du graphe

// Lit un fichier texte et construit la liste d’adjacence correspondante
 
t_list_adj readGraph(const char *filename)   //a faire 