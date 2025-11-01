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
    list_adj.tab = (t_list *)malloc(size * sizeof(t_list));

    if (list_adj.tab == NULL) {
        printf("Erreur : allocation mémoire du tableau de listes.\n");
        list_adj.size = 0;
        return list_adj;
    }

    // On initialise chaque liste du tableau comme vide
    for (int i = 0; i < size; i++) {
        t_list *temp = createEmptyList();
        list_adj.tab[i] = *temp;
        free(temp);
    }

    return list_adj;
}
 
// Affiche une liste d’adjacence complète
void displayListAdj(t_list_adj *list_adj) {
    if (list_adj == NULL || list_adj->tab == NULL) {
        printf("Liste d'adjacence vide ou non initialisée.\n");
        return;
    }

    printf("Affichage de la liste d’adjacence\n");
    for (int i = 0; i < list_adj->size; i++) {
        printf("Liste pour le sommet %d : ", i);
        displayList(&(list_adj->tab[i]));
        printf("\n");
    }
}

// Lecture du fichier et création du graphe

// Lit un fichier texte et construit la liste d’adjacence correspondante
 
t_list_adj readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, depart, arrivee;
    float proba;

    t_list_adj list_adj;

    if (file==NULL){
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    // Première ligne contient le nombre de sommets
    if (fscanf(file, "%d", &nbvert) != 1){
        perror("Could not read number of vertices");
        exit(EXIT_FAILURE);
    }

    list_adj = createEmptyAdjList(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3){
        // on obtient, pour chaque ligne du fichier les valeurs
        // depart, arrivee, et proba
        addCell(&(list_adj.liste[depart]), arrivee, proba);
    }

    fclose(file);

    return list_adj;
} 