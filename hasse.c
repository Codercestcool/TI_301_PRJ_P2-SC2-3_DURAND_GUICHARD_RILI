#include "hasse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Crée et initialise un tableau dynamique de liens.
/* Crée un tableau dynamique de liens entre classes.
   Cette fonction alloue une structure t_link_array et un tableau initial de t_link.
   Elle initialise la taille à 0 et la capacité selon la valeur donnée.
   Le but est de préparer une structure extensible pour stocker les relations inter-classes. */
t_link_array *create_link_array(int initial_capacity) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Error: initial_capacity (%d) invalid in create_link_array.\n",
                initial_capacity);
        return NULL;
    }

    t_link_array *arr = (t_link_array *)malloc(sizeof(t_link_array));
    if (!arr) {
        perror("Error: malloc failed for t_link_array");
        exit(EXIT_FAILURE);
    }

    arr->links = (t_link *)malloc(sizeof(t_link) * initial_capacity);
    if (!arr->links) {
        perror("Error: malloc failed for links");
        free(arr);
        exit(EXIT_FAILURE);
    }

    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

//Libère la mémoire allouée pour le tableau de liens.
/* Libère entièrement la mémoire allouée par un tableau de liens.
   Elle libère d'abord le tableau de t_link, puis la structure t_link_array elle-même.
   Son rôle est simplement de nettoyer correctement la mémoire une fois le travail terminé. */

void free_link_array(t_link_array *arr) {
    if (arr) {
        free(arr->links);
        free(arr);
    }
}

//Vérifie si un lien entre deux classes existe déjà.
/* Vérifie si un lien source → destination existe déjà dans le tableau.
   Elle parcourt la liste actuelle de liens et retourne 1 si déjà présent, 0 sinon.
   Cela permet d'éviter les doublons lors de la création du diagramme. */

int link_exists(t_link_array *arr, int source_id, int dest_id) {
    if (!arr) return 0;
    for (int i = 0; i < arr->size; i++) {
        if (arr->links[i].source_class_id == source_id && arr->links[i].dest_class_id == dest_id)
            return 1;
    }
    return 0;
}

//Ajoute un lien au tableau si non présent, gère la réallocation.
/* Ajoute un lien source → destination dans le tableau dynamique si il n'existe pas déjà.
   La fonction vérifie l'existence, réalloue le tableau en cas de manque d'espace, puis ajoute le lien.
   Elle construit progressivement la structure représentant la relation entre classes. */

void add_link(t_link_array *arr, int source_id, int dest_id) {
    if (!arr) return;
    if (link_exists(arr, source_id, dest_id)) return; // évite doublon

    if (arr->size == arr->capacity) {
        int new_capacity = arr->capacity * 2;
        if (new_capacity < 4) new_capacity = 4;
        t_link *tmp = (t_link *)realloc(arr->links, sizeof(t_link) * new_capacity);
        if (!tmp) {
            perror("add_link: realloc failed");
            return;
        }
        arr->links = tmp;
        arr->capacity = new_capacity;
    }

    arr->links[arr->size].source_class_id = source_id;
    arr->links[arr->size].dest_class_id = dest_id;
    arr->size++;
}

//Construit les liens entre classes (Diagramme de Hasse).
/* Calcule les liens entre classes à partir du graphe et de la partition obtenue (CFCs).
   Pour chaque arête du graphe original, si l'arête relie deux classes différentes,
   un lien inter-classes est ajouté. Cela construit la base du diagramme de Hasse. */

t_link_array *compute_hasse_diagram_links(t_graph graph, t_partition partition) {
    if (partition.num_classes == 0) return NULL;

    // Initialisation du tableau de liens (capacité initiale = N liens max)
    t_link_array *links = create_link_array(partition.num_classes * partition.num_classes);
    if (!links) return NULL;

    for (int i = 0; i < graph.num_vertices; ++i) {
        // u_id est le sommet de départ (1-based)
        int u_id = i + 1;
        // u_class_id est la classe de départ (1-based)
        int u_class_id = partition.v_data[i].class_id;

        // Parcourir les arêtes sortantes du sommet u
        t_edge *current_edge = graph.adj_lists[i].head;
        while (current_edge != NULL) {
            int v_id = current_edge->destination;
            int v_idx = v_id - 1;
            // v_class_id est la classe d'arrivée (1-based)
            int v_class_id = partition.v_data[v_idx].class_id;

            // Il y a un lien inter-classes si les IDs sont différents
            if (u_class_id != v_class_id) {
                add_link(links, u_class_id, v_class_id);
            }
            current_edge = current_edge->next;
        }
    }
    return links;
}


//Génère le fichier Mermaid pour visualiser le Diagramme de Hasse.
/* Génère un fichier Mermaid (.md) contenant la représentation graphique du diagramme de Hasse.
   Cette fonction écrit les nœuds (classes persistantes ou transitoires)
   puis les liens entre classes sous forme d'instructions Mermaid.
   Le but est de permettre une visualisation claire des relations entre classes. */

void generate_hasse_mermaid_file(t_link_array *links, const char *output_filename, t_partition partition) {
    FILE *file = fopen(output_filename, "w");

    if (file == NULL) {
        perror("Could not open output file for Hasse writing");
        return;
    }

    fprintf(file, "--- \n");
    fprintf(file, "config:\n");
    fprintf(file, "layout: elk\n");
    fprintf(file, "theme: neutral\n");
    fprintf(file, "---\n");
    fprintf(file, "flowchart TD\n"); // De haut en bas (top-down) pour les classes

    // 1. Définition des nœuds (Classes) avec leur type
    for (int i = 0; i < partition.num_classes; i++) {
        t_class c = partition.classes[i];

        // Utilisation de la forme C1((C1 Persistante)) ou C2[C2 Transitoire]
        const char *label_type = c.is_persistent ? "Persistante" : "Transitoire";
        const char *shape_start = c.is_persistent ? "((" : "[";
        const char *shape_end = c.is_persistent ? "))" : "]";

        // Ex: C1((C1 Persistante))
        fprintf(file, "C%d%sC%d %s%s\n",
                c.id, shape_start,
                c.id, label_type,
                shape_end);
    }

    // 2. Définition des liens
    for (int i = 0; i < links->size; i++) {
        // Ex: C1 --> C2
        fprintf(file, "C%d --> C%d\n",
                links->links[i].source_class_id,
                links->links[i].dest_class_id);
    }

    fclose(file);
    printf("Fichier mermaid du diagramme de Hasse genere: %s\n", output_filename);
}

// --- OPTIONNEL : Suppression des liens transitifs ---

//Recherche récursive de chemin (DFS) entre deux classes dans le graphe des classes.
/* Vérifie récursivement l'existence d'un chemin entre deux classes.
   La fonction recherche si, depuis une classe de départ, il existe une suite de liens
   menant à une classe d'arrivée, en excluant le lien direct initial.
   Elle permet de détecter des relations transitives dans le diagramme. */

static int path_exists_from(const t_link_array *arr, int start, int end) {
    for (int i = 0; i < arr->size; i++) {
        if (arr->links[i].source_class_id == start) {
            int mid = arr->links[i].dest_class_id;
            // Ne pas vérifier immédiatement la fin, on cherche un chemin intermédiaire
            if (mid == end) continue; // Si A->B est le lien i, on ne veut pas considérer A->B comme le chemin A->X->B

            // Si le lien intermédiaire A → M existe, on vérifie si M → ... → B
            if (path_exists_from(arr, mid, end)) return 1;
        }
    }
    return 0;
}

//Supprime les liens redondants pour obtenir un Diagramme de Hasse strict.
/* Supprime les liens transitifs pour obtenir un diagramme de Hasse strict.
   Un lien A→B est retiré si un chemin A→M→…→B existe via une autre classe.
   La fonction garde un tableau des liens à conserver, élimine les redondants,
   et reconstruit une liste nettoyée, ne laissant que les relations minimales. */

void remove_transitive_links(t_link_array *links) {
    if (!links || links->size < 2) return;

    int size = links->size;
    int *keep = (int *)malloc(sizeof(int) * size);
    if (!keep) {
        perror("malloc failed for 'keep' array");
        return;
    }
    for (int i = 0; i < size; i++) keep[i] = 1;

    for (int i = 0; i < size; i++) {
        int A = links->links[i].source_class_id;
        int B = links->links[i].dest_class_id;

        // On cherche A → M → ... → B (où M est une classe intermédiaire).
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (!keep[i]) break;

            int A2 = links->links[j].source_class_id;
            int M  = links->links[j].dest_class_id;

            // Si le lien j est de la forme A → M et que M n'est pas B
            if (A == A2 && M != B) {
                // On vérifie s'il existe un chemin M -> ... -> B
                if (path_exists_from(links, M, B)) {
                    keep[i] = 0; // Lien A -> B est redondant
                    break;
                }
            }
        }
    }

    // Reconstruction filtrée du tableau de liens
    int new_size = 0;
    for (int i = 0; i < size; i++) {
        if (keep[i]) {
            links->links[new_size++] = links->links[i];
        }
    }

    links->size = new_size;
    free(keep);
}


 /*Résumé simple

1. On crée un **tableau dynamique de liens**.
2. On parcourt le graphe pour générer tous les **liens inter-classes**.
3. On **supprime les liens transitifs** pour obtenir le diagramme de Hasse strict.
4. On génère un fichier **Mermaid** pour visualiser les classes et leurs relations.


Le code “Hasse” sert à **construire automatiquement le diagramme de Hasse entre les classes fortement connexes (CFC)
** obtenues après Tarjan. Concrètement, une fois que le graphe a été regroupé en classes, ce module analyse toutes 
les arêtes du graphe pour déterminer **quelles classes dépendent d’autres classes**, puis génère des liens entre elles. 
    Ensuite, il filtre ces liens pour supprimer ceux qui sont **transitifs** (par exemple si A → B et B → C existent, on enlève A → C), 
afin que le résultat soit un **diagramme de Hasse strict**, c’est-à-dire une représentation claire et minimale de la relation 
d’ordre entre classes. Enfin, il génère un fichier **Mermaid** qui permet de visualiser ce diagramme sous forme de graphe
hiérarchique lisible. En résumé : ce code prend les CFC et construit la **structure hiérarchique** entre elles, puis la
rend **visible et exploitable**.
*/
