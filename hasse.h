#ifndef HASSE_H
#define HASSE_H

#include "graph.h"    // Pour t_graph
#include "tarjan.h"   // Pour t_partition et t_class

typedef struct {
    int source_class_id; // ID classe de départ (basé sur 1)
    int dest_class_id;   // ID classe d'arrivée (basé sur 1)
} t_link;

typedef struct {
    t_link *links;   // tableau de liens
    int size;        // nombre de liens utilisés
    int capacity;    // capacité actuelle du tableau
} t_link_array;


//Crée et initialise un tableau dynamique de liens.
t_link_array *create_link_array(int initial_capacity);

//Vérifie si un lien entre deux classes existe déjà.
int link_exists(t_link_array *arr, int source_id, int dest_id);

//Ajoute un lien au tableau si non présent, gère la réallocation.
void add_link(t_link_array *arr, int source_id, int dest_id);

//Libère la mémoire allouée pour le tableau de liens.

void free_link_array(t_link_array *arr);


//Détermine la nature de chaque classe et met à jour la structure t_partition.
void analyze_class_types(t_graph graph, t_partition *partition);

//Construit les liens entre classes (Diagramme de Hasse).
t_link_array *compute_hasse_diagram_links(t_graph graph, t_partition partition);

//Génère le fichier Mermaid pour visualiser le Diagramme de Hasse.
void generate_hasse_mermaid_file(t_link_array *links, const char *output_filename, t_partition partition);

//Supprime les liens redondants pour obtenir un Diagramme de Hasse strict.
void remove_transitive_links(t_link_array *links);

#endif // HASSE_H