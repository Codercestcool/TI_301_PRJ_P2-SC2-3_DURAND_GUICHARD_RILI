#ifndef GRAPH_H
#define GRAPH_H

//Représente une arête (ou une 'cellule' dans la liste chaînée). Chaque arête a une probabilité et mène à un sommet.
typedef struct s_edge {
    int destination;// Sommet d'arrivée
    float probability;// Probabilité associée
    struct s_edge *next;// Pointeur vers la cellule suivante
} t_edge;

//Représente une liste d'arêtes sortantes pour un sommet. Stocke toutes les arêtes 'sortant' d'un sommet.
typedef struct s_list {
    t_edge *head;// Pointeur vers la première cellule/arête
} t_list;

//Représente la liste d'adjacence complète du graphe. C'est un tableau dynamique de 'liste' avec sa taille.
typedef struct s_graph {
    t_list *adj_lists;// Tableau dynamique de listes d'adjacence (1 par sommet)
    int num_vertices; // Nombre de sommets (taille du tableau)
} t_graph;

//Crée et initialise une nouvelle cellule/arête.
t_edge *create_edge(int arrival, float proba);

//Crée et initialise une liste d'arêtes vide.
t_list create_empty_list();

//Ajoute une nouvelle arête (cellule) au début d'une liste.
void add_edge_to_list(t_list *list, t_edge *edge);

//Crée et initialise une liste d'adjacence 'vide' à partir d'une taille donnée.
t_graph create_empty_graph(int num_vertices);

//Affiche le contenu d'une liste d'adjacence (une par une pour chaque sommet).
void display_graph(t_graph graph);

//Lit un fichier et construit la liste d'adjacence.
t_graph read_graph(const char *filename);

//Libère la mémoire allouée pour le graphe.
void free_graph(t_graph graph);

#endif // GRAPH_H