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

t_cell * createCell(int arrivee, float proba) {
    t_cell *cell = (t_cell *)malloc(sizeof(t_cell));
    cell->arrivee = arrivee;
    cell->proba = proba;
    return cell;
}

t_list * createEmptyList() {
    t_list *list = (t_list *)malloc(sizeof(t_list));
    list->head = NULL;
    return list;
}

void addCell(t_list *list, t_cell *cell) {
    cell->suiv = NULL;
    if (list->head == NULL) {
        list->head = cell;
    } else {
        t_cell *p = list->head;
        while (p->suiv != NULL) {
            p = p->suiv;
        }
        p->suiv = cell;
    }
}

void displayList(t_list *list) {
    t_cell *cell = list->head;
    printf("[head @] -> ");
    while (cell->suiv != NULL) {
        printf("(%d, %f) -> ", cell->arrivee, cell->proba);
        cell = cell->suiv;
    }
    printf("(%d, %f)", cell->arrivee, cell->proba);
}