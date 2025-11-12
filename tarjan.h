#ifndef __TARJAN_H__
#define __TARJAN_H__

// PARTIE 2
typedef struct s_tarjan_sommet {
    int id; // Numéro du sommet dans le graphe
    int num; // Numéro temporaire du sommet (pour créer la classe)
    int num_acc; // Numéro du sommet accessible
    int booleen; // Valant soit 0 soit 1
} t_tarjan_sommet;

typedef struct s_classe {
    int *sommets;   // Tableau des sommets de la classe
    int taille;     // Nombre de sommets dans cette classe
} t_classe;

typedef struct s_classe_list {
    t_classe *classes; // Tableau de classes
    int nb_classes;    // Nombre de classes trouvées
    int capacite;      // Capacité allouée pour agrandir si besoin
} t_classe_list;

t_tarjan_sommet * tab_sommets(t_list_adj graph);
void parcours(t_list_adj *graph, t_tarjan_sommet *sommets, int v, t_classe_list *partition);
t_classe_list *tarjan(t_list_adj *graph);

#endif  