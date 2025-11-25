#include "graph.h"

/*  
   create_edge :
   Alloue et initialise une nouvelle arête (cellule) avec le sommet d'arrivée
   et la probabilité associée. La prochaine arête est mise à NULL.
*/
t_edge *create_edge(int arrival, float proba) {
    t_edge *new_edge = (t_edge *)malloc(sizeof(t_edge));
    if (new_edge == NULL) {
        perror("Allocation failed for t_edge");
        exit(EXIT_FAILURE);
    }
    new_edge->destination = arrival;
    new_edge->probability = proba;
    new_edge->next = NULL;
    return new_edge;
}

/*  
   create_empty_list :
   Crée et initialise une liste d'arêtes vide (head = NULL).
*/
t_list create_empty_list() {
    t_list new_list;
    new_list.head = NULL;
    return new_list;
}

/*  
   add_edge_to_list :
   Ajoute une arête au début de la liste d'adjacence.
   Permet de construire rapidement la liste pour un sommet donné.
*/
void add_edge_to_list(t_list *list, t_edge *edge) {
    if (list == NULL || edge == NULL) return;
    edge->next = list->head;
    list->head = edge;
}

/*  
   create_empty_graph :
   Crée un graphe vide avec un nombre donné de sommets.
   Alloue un tableau de listes d'adjacence et initialise chaque liste vide.
*/
t_graph create_empty_graph(int num_vertices) {
    t_graph graph;
    graph.num_vertices = num_vertices;
    graph.adj_lists = (t_list *)calloc(num_vertices, sizeof(t_list));
    
    if (graph.adj_lists == NULL) {
        perror("Allocation failed for adj_lists");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_vertices; i++) {
        graph.adj_lists[i] = create_empty_list();
    }
    return graph;
}

/*  
   display_graph :
   Affiche le graphe sous forme de liste d'adjacence.
   Chaque ligne montre un sommet et toutes ses arêtes (destination et probabilité).
*/
void display_graph(t_graph graph) {
    printf("--- Affichage de la liste d'adjacence ---\n");
    for (int i = 0; i < graph.num_vertices; i++) {
        printf("Sommet %d : ", i + 1); 
        t_edge *current = graph.adj_lists[i].head;
        while (current != NULL) {
            printf("-> (%d, %.2f) ", current->destination, current->probability);
            current = current->next;
        }
        printf("\n");
    }
    printf("----------------------------------------\n");
}

/*  
   free_graph :
   Libère toute la mémoire allouée pour le graphe.
   Parcourt chaque liste et libère chaque arête, puis le tableau de listes.
*/
void free_graph(t_graph graph) {
    for (int i = 0; i < graph.num_vertices; i++) {
        t_edge *current = graph.adj_lists[i].head;
        while (current != NULL) {
            t_edge *to_free = current;
            current = current->next;
            free(to_free);
        }
        graph.adj_lists[i].head = NULL;
    }
    free(graph.adj_lists);
    graph.adj_lists = NULL;
    graph.num_vertices = 0;
}

/*  
   read_graph :
   Lit un fichier de graphe au format texte et construit le graphe correspondant.
   La première ligne doit contenir le nombre de sommets, les suivantes contiennent
   les arêtes sous forme : départ arrivée probabilité.
   Retourne le graphe complet sous forme de liste d'adjacence.
*/
t_graph read_graph(const char *filename) {
    FILE *file = fopen(filename, "rt"); 
    int nbvert, depart, arrivee;
    float proba;
    t_graph graph = {NULL, 0};

    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    graph = create_empty_graph(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
            fprintf(stderr, "Error: Invalid vertex number (%d or %d) found in file.\n", depart, arrivee);
            free_graph(graph);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        t_edge *new_edge = create_edge(arrivee, proba);
        add_edge_to_list(&graph.adj_lists[depart - 1], new_edge);
    }

    fclose(file);
    return graph;
}
