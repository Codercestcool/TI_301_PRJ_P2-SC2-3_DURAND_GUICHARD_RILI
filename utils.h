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


// PARTIE 1 ETAPE 1
t_cell * createCell(int arrivee, float proba);
t_list * createEmptyList();
void addCell(t_list *list, int arrivee, float proba);
void displayList(t_list *list);
t_list_adj createEmptyAdjList(int size);
void displayListAdj(t_list_adj *list_adj);
t_list_adj readGraph(const char *filename);

// PARTIE 1 ETAPE 2
void verifyMarkovGraph(t_list_adj *graph);

// PARTIE 1 ETAPE 3
void exportToMermaid(t_list_adj *graph, const char *filename);


// PARTIE 2
typedef struct s_tarjan_sommet {
    int id; // Numéro du sommet dans le graphe
    int num; // Numéro temporaire du sommet (pour créer la classe)
    int num_acc; // Numéro du sommet accessible
    int booleen; // Valant soit 0 soit 1
} t_tarjan_sommet;

typedef struct s_classe {
    t_tarjan_sommet *sommets_classe = malloc(n * sizeof(t_tarjan_sommet));
} t_classe;

typedef struct s_classe_list {
    t_classe *classe = malloc(n * sizeof(t_classe));
} t_classe_list;

t_tarjan_sommet * tab_sommets(t_list_adj graph);
void parcours(t_tarjan_sommet sommet);


#endif