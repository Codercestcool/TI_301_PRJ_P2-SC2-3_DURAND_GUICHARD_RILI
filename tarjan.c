#include "tarjan.h"
#include <stdio.h>
#include <stdlib.h>

// Variables globales pour l'algorithme de Tarjan (pour simplifier la récursivité)
static int current_time = 0;
static t_stack vertex_stack;
static t_partition *global_partition = NULL;
static t_graph *global_graph = NULL;

t_stack create_stack(int capacity) {
    t_stack stack;
    stack.capacity = capacity;
    stack.top = -1;
    stack.data = (int *)malloc(capacity * sizeof(int));
    if (stack.data == NULL) {
        perror("Stack allocation failed");
        exit(EXIT_FAILURE);
    }
    return stack;
}

void push(t_stack *stack, int vertex_id) {
    if (stack->top >= stack->capacity - 1) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
    stack->data[++stack->top] = vertex_id;
}

int pop(t_stack *stack) {
    if (stack->top < 0) {
        return -1; // Pile vide
    }
    return stack->data[stack->top--];
}

void free_stack(t_stack stack) {
    free(stack.data);
}

//Ajoute une CFC nouvellement trouvée à la partition.
static void add_new_class(int v_id) {
    // 1. Allouer de l'espace pour la nouvelle classe
    global_partition->num_classes++;
    global_partition->classes = (t_class *)realloc(
        global_partition->classes,
        global_partition->num_classes * sizeof(t_class)
    );
    if (global_partition->classes == NULL) {
        perror("Realloc failed for classes");
        exit(EXIT_FAILURE);
    }

    t_class *new_class = &global_partition->classes[global_partition->num_classes - 1];
    new_class->id = global_partition->num_classes; // ID de classe
    new_class->num_members = 0;
    new_class->members_ids = NULL;
    new_class->is_persistent = 0; // Initialisation par défaut

    int current_vertex_id;
    int temp_members[global_partition->v_data[0].vertex_id]; // Max size N

    // 2. Dépiler les sommets de la CFC et les ajouter à la nouvelle classe
    do {
        current_vertex_id = pop(&vertex_stack); // ID de sommet (1 à N)
        if (current_vertex_id == -1) break; // Sécurité

        int index = current_vertex_id - 1; // Index 0 à N-1

        // Marquer le sommet comme attribué à cette classe
        global_partition->v_data[index].on_stack = 0;
        global_partition->v_data[index].class_id = new_class->id;

        temp_members[new_class->num_members++] = current_vertex_id;
    } while (current_vertex_id != v_id);

    // 3. Stocker les membres de la classe dans l'ordre
    new_class->members_ids = (int *)malloc(new_class->num_members * sizeof(int));
    if (new_class->members_ids == NULL) {
        perror("Members_ids allocation failed");
        exit(EXIT_FAILURE);
    }
    // Copie de l'ordre de la pile
    for (int i = 0; i < new_class->num_members; i++) {
        new_class->members_ids[i] = temp_members[i];
    }
}

//Fonction récursive du parcours en profondeur de Tarjan.
static void tarjan_dfs(int u_id) {
    int u_idx = u_id - 1;

    // 1. Initialisation : num et low du sommet u
    global_partition->v_data[u_idx].num = current_time;
    global_partition->v_data[u_idx].low = current_time;
    current_time++;

    push(&vertex_stack, u_id);
    global_partition->v_data[u_idx].on_stack = 1;

    // 2. Parcourir les voisins v
    t_edge *current_edge = global_graph->adj_lists[u_idx].head;
    while (current_edge != NULL) {
        int v_id = current_edge->destination;
        int v_idx = v_id - 1;

        if (global_partition->v_data[v_idx].num == -1) {
            // Voisin v non visité : appel récursif
            tarjan_dfs(v_id);
            // Mise à jour de low[u] : le plus petit low_link atteint via un descendant
            if (global_partition->v_data[v_idx].low < global_partition->v_data[u_idx].low) {
                global_partition->v_data[u_idx].low = global_partition->v_data[v_idx].low;
            }
        } else if (global_partition->v_data[v_idx].on_stack) {
            // Voisin v déjà visité ET sur la pile : arête de retour
            // Mise à jour de low[u] : num[v] est potentiellement un ancêtre dans la CFC
            if (global_partition->v_data[v_idx].num < global_partition->v_data[u_idx].low) {
                global_partition->v_data[u_idx].low = global_partition->v_data[v_idx].num;
            }
        }
        current_edge = current_edge->next;
    }

    // 3. Détection de la CFC
    if (global_partition->v_data[u_idx].low == global_partition->v_data[u_idx].num) {
        // u est la racine d'une CFC : tous les sommets jusqu'à u sont dans cette CFC
        add_new_class(u_id);
    }
}

//Implémente l'algorithme de Tarjan pour trouver toutes les CFCs.

t_partition find_cfcs_tarjan(t_graph graph) {
    int N = graph.num_vertices;
    t_partition partition;

    // Initialisation
    partition.num_classes = 0;
    partition.classes = NULL;
    partition.v_data = (t_tarjan_vertex *)calloc(N, sizeof(t_tarjan_vertex));

    if (partition.v_data == NULL) {
        perror("Partition v_data allocation failed");
        exit(EXIT_FAILURE);
    }

    // Préparation des données Tarjan pour chaque sommet
    for (int i = 0; i < N; i++) {
        partition.v_data[i].vertex_id = i + 1;
        partition.v_data[i].num = -1; // Non visité
        partition.v_data[i].low = -1; // Non visité
        partition.v_data[i].on_stack = 0;
        partition.v_data[i].class_id = 0; // Non attribué
    }

    // Initialisation des variables globales pour le parcours
    current_time = 0;
    vertex_stack = create_stack(N);
    global_partition = &partition;
    global_graph = &graph;

    // Appel de Tarjan pour tous les sommets (assure que les graphes déconnectés sont traités)
    for (int i = 0; i < N; i++) {
        if (partition.v_data[i].num == -1) {
            tarjan_dfs(i + 1);
        }
    }

    // Nettoyage des variables globales et de la pile
    free_stack(vertex_stack);
    global_partition = NULL;
    global_graph = NULL;

    return partition;
}

//@brief Affiche la partition complète.

void display_partition(t_partition partition) {
    printf("--- Partition en Classes (CFCs) ---\n");
    printf("Nombre total de classes: %d\n", partition.num_classes);
    for (int i = 0; i < partition.num_classes; i++) {
        t_class c = partition.classes[i];
        printf("Classe C%d (%s) [ID: %d, Taille: %d] : { ",
               c.id,
               c.is_persistent ? "Persistante" : "Transitoire",
               c.id,
               c.num_members);

        for (int j = 0; j < c.num_members; j++) {
            printf("%d%s", c.members_ids[j], (j < c.num_members - 1) ? ", " : "");
        }
        printf(" }\n");
    }
    printf("-----------------------------------\n");
}

//Libère la mémoire allouée pour la structure de partition.
void free_partition(t_partition partition) {
    if (partition.v_data != NULL) {
        free(partition.v_data);
        partition.v_data = NULL;
    }
    if (partition.classes != NULL) {
        for (int i = 0; i < partition.num_classes; i++) {
            if (partition.classes[i].members_ids != NULL) {
                free(partition.classes[i].members_ids);
                partition.classes[i].members_ids = NULL;
            }
        }
        free(partition.classes);
        partition.classes = NULL;
    }
    partition.num_classes = 0;
}