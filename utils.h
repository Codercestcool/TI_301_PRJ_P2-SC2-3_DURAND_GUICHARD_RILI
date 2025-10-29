#ifndef __UTILS_H__
#define __UTILS_H__

// Structure où chaque cellule correspond à une transition)
typedef struct s_cell {
    int arrivee; // Numéro du sommet d'arrivée
    float proba; // Probabilité de transition
    struct s_cell *suivant; // Pointeur vers la cellule suivante (liste chaînée)
} t_cell;

// Structure représentant la liste des arêtes sortantes d’un sommet
typedef struct s_list {
    t_cell *head; // Pointeur vers le début de la liste
} t_list;

// Structure principale : liste d’adjacence complète
// Contient un tableau de "list" et la taille du graphe (nombre de sommets)
typedef struct s_list_adj {
    int taille; // Nombre de sommets
    t_list *liste; // Tableau de listes (une par sommet)
} t_list_adj;

static char *getID(int i);

t_cell * createCell(int arrivee, float proba);
t_list * createEmptyList();
void addCell(t_list *list, int arrivee, float proba);
void displayList(t_list *list);
t_list_adj * createListAdj(int taille);
void displayListAdj(t_list_adj *list_adj);


#endif