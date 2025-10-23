#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct s_cell {
    int arrivee;
    float proba;
    struct s_cell *suiv;
} t_cell;

typedef struct s_list {
    t_cell *head;
} t_list;

typedef struct s_list_adj {
    int taille;
    t_list *liste;
} t_list_adj;

static char *getID(int i);

t_cell * createCell(int arrivee, float proba);
t_list * createEmptyList();
void addCell(t_list *list, t_cell *cell);
void displayList(t_list *list);
t_list_adj * createListAdj(int taille);
void displayListAdj(t_list_adj *list_adj);


#endif