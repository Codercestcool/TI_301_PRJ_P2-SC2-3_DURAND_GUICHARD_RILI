#include "graph.h"

//Crée et initialise une nouvelle cellule/arête.
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

//Crée et initialise une liste d'arêtes vide.
t_list create_empty_list() {
    t_list new_list;
    new_list.head = NULL;
    return new_list;
}

//Ajoute une nouvelle arête (cellule) au début d'une liste.
void add_edge_to_list(t_list *list, t_edge *edge) {
    if (list == NULL || edge == NULL) return;
    edge->next = list->head;
    list->head = edge;
}

//Crée et initialise une liste d'adjacence 'vide' à partir d'une taille donnée.
t_graph create_empty_graph(int num_vertices) {
    t_graph graph;
    graph.num_vertices = num_vertices;
    // Allocation pour le tableau de listes (une par sommet)
    graph.adj_lists = (t_list *)calloc(num_vertices, sizeof(t_list));
    
    if (graph.adj_lists == NULL) {
        perror("Allocation failed for adj_lists");
        exit(EXIT_FAILURE);
    }

    // Initialisation de chaque liste dans le tableau comme liste vide
    for (int i = 0; i < num_vertices; i++) {
        graph.adj_lists[i] = create_empty_list();
    }
    return graph;
}

//Affiche le contenu d'une liste d'adjacence.

void display_graph(t_graph graph) {
    printf("--- Affichage de la liste d'adjacence ---\n");
    for (int i = 0; i < graph.num_vertices; i++) {
        // Les sommets sont numérotés de 1 à N.
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

/**
 * @brief Libère la mémoire allouée pour le graphe.
 */
void free_graph(t_graph graph) {
    for (int i = 0; i < graph.num_vertices; i++) {
        t_edge *current = graph.adj_lists[i].head;
        while (current != NULL) {
            t_edge *to_free = current;
            current = current->next;
            free(to_free);
        }
        graph.adj_lists[i].head = NULL; // Sécurité
    }
    free(graph.adj_lists);
    graph.adj_lists = NULL;
    graph.num_vertices = 0;
}


//Lit un fichier de données et construit la liste d'adjacence.
t_graph read_graph(const char *filename) {
    FILE *file = fopen(filename, "rt"); 
    int nbvert, depart, arrivee;
    float proba;
    t_graph graph = {NULL, 0}; // Initialisation par défaut

    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    // Lire la première ligne : nombre de sommets
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Initialiser une liste d’adjacence vide à partir du nombre de sommets
    graph = create_empty_graph(nbvert); 

    // Lire les lignes suivantes : sommet_de_départ, sommet_d_arrivée, probabilité
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        // Validation basique (les sommets doivent exister)
        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
            fprintf(stderr, "Error: Invalid vertex number (%d or %d) found in file.\n", depart, arrivee);
            free_graph(graph);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        // Ajouter l’arête : 'depart' est l'indice (depart - 1) dans le tableau.
        t_edge *new_edge = create_edge(arrivee, proba);
        add_edge_to_list(&graph.adj_lists[depart - 1], new_edge);
    }

    fclose(file);
    return graph;// retourne la liste d’adjacence remplie
}