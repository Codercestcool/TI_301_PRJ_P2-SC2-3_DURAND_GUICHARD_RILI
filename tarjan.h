#ifndef TARJAN_H
#define TARJAN_H

#include "graph.h"

//Représente les données nécessaires pour l'algorithme de Tarjan pour chaque sommet.
typedef struct s_tarjan_vertex {
    int vertex_id;  // Numéro du sommet (1 à N)
    int num;        // Numéro de parcours, -1 si non visité
    int low;        // Numéro accessible minimal, -1 si non visité
    int on_stack;   // Indicateur booléen : 1 si sur la pile, 0 sinon
    int class_id;   // ID de la classe à laquelle il appartient, 0 si non attribué
} t_tarjan_vertex;

//Représente une seule classe (CFC)

typedef struct s_class {
    int id;               // ID unique de la classe
    int num_members;      // Nombre de sommets dans la classe
    int *members_ids;     // Tableau des numéros de sommets
    int is_persistent;    // 1 si la classe est persistante, 0 si transitoire
} t_class;

//Représente l'ensemble des classes trouvées (la partition du graphe).
typedef struct s_partition {
    t_class *classes;      // Tableau dynamique des classes
    int num_classes;       // Nombre total de classes trouvées
    t_tarjan_vertex *v_data; // Tableau des données Tarjan pour N sommets
} t_partition;

//Représente une pile simple pour Tarjan.
typedef struct s_stack {
    int *data;
    int capacity;
    int top;
} t_stack;


t_stack create_stack(int capacity);
void push(t_stack *stack, int vertex_id);
int pop(t_stack *stack);
void free_stack(t_stack stack);

//Implémente l'algorithme de Tarjan pour trouver toutes les CFCs.
t_partition find_cfcs_tarjan(t_graph graph);

//Affiche la partition complète (toutes les classes trouvées).
void display_partition(t_partition partition);

//Libère la mémoire allouée pour la structure de partition.
void free_partition(t_partition partition);

#endif // TARJAN_H