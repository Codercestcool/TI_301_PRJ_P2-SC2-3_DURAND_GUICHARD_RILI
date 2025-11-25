 #include "tarjan.h"
#include <stdio.h>
#include <stdlib.h>

/*  
   Variables globales utilisées par Tarjan pour gérer plus facilement 
   la récursivité : un compteur de temps, une pile de sommets,
   et des pointeurs vers la partition et le graphe en cours de traitement.
*/
static int current_time = 0;
static t_stack vertex_stack;
static t_partition *global_partition = NULL;
static t_graph *global_graph = NULL;

/*  
   create_stack :
   Initialise une pile capable de contenir un nombre donné d'éléments.
   Alloue la mémoire et prépare top = -1 pour indiquer qu’elle est vide.
*/
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

/*  
   push :
   Ajoute un sommet sur la pile si elle n'est pas pleine.
   La pile sert à stocker les sommets actifs dans la CFC en construction.
*/
void push(t_stack *stack, int vertex_id) {
    if (stack->top >= stack->capacity - 1) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
    stack->data[++stack->top] = vertex_id;
}

/*  
   pop :
   Retire et renvoie l’élément situé au sommet de la pile.
   Retourne -1 si la pile est vide.
*/
int pop(t_stack *stack) {
    if (stack->top < 0) {
        return -1; // Pile vide
    }
    return stack->data[stack->top--];
}

/*  
   free_stack :
   Libère la mémoire allouée pour la pile.
*/
void free_stack(t_stack stack) {
    free(stack.data);
}

/*  
   add_new_class :
   Fonction appelée quand Tarjan découvre la racine d’une CFC.
   Elle dépile tous les sommets appartenant à cette CFC, crée une nouvelle classe,
   leur assigne un identifiant de classe et les stocke dans la partition.
*/
static void add_new_class(int v_id) {
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
    new_class->id = global_partition->num_classes;
    new_class->num_members = 0;
    new_class->members_ids = NULL;
    new_class->is_persistent = 0;

    int current_vertex_id;
    int temp_members[global_partition->v_data[0].vertex_id];

    do {
        current_vertex_id = pop(&vertex_stack);
        if (current_vertex_id == -1) break;

        int index = current_vertex_id - 1;

        global_partition->v_data[index].on_stack = 0;
        global_partition->v_data[index].class_id = new_class->id;

        temp_members[new_class->num_members++] = current_vertex_id;
    } while (current_vertex_id != v_id);

    new_class->members_ids = (int *)malloc(new_class->num_members * sizeof(int));
    if (new_class->members_ids == NULL) {
        perror("Members_ids allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < new_class->num_members; i++) {
        new_class->members_ids[i] = temp_members[i];
    }
}

/*  
   tarjan_dfs :
   Fonction récursive centrale de l’algorithme de Tarjan.
   Attribue num et low, explore les voisins, détecte les arcs de retour,
   et identifie la racine d’une CFC pour déclencher sa création.
*/
static void tarjan_dfs(int u_id) {
    int u_idx = u_id - 1;

    global_partition->v_data[u_idx].num = current_time;
    global_partition->v_data[u_idx].low = current_time;
    current_time++;

    push(&vertex_stack, u_id);
    global_partition->v_data[u_idx].on_stack = 1;

    t_edge *current_edge = global_graph->adj_lists[u_idx].head;
    while (current_edge != NULL) {
        int v_id = current_edge->destination;
        int v_idx = v_id - 1;

        if (global_partition->v_data[v_idx].num == -1) {
            tarjan_dfs(v_id);
            if (global_partition->v_data[v_idx].low < global_partition->v_data[u_idx].low) {
                global_partition->v_data[u_idx].low = global_partition->v_data[v_idx].low;
            }
        } else if (global_partition->v_data[v_idx].on_stack) {
            if (global_partition->v_data[v_idx].num < global_partition->v_data[u_idx].low) {
                global_partition->v_data[u_idx].low = global_partition->v_data[v_idx].num;
            }
        }
        current_edge = current_edge->next;
    }

    if (global_partition->v_data[u_idx].low == global_partition->v_data[u_idx].num) {
        add_new_class(u_id);
    }
}

/*  
   find_cfcs_tarjan :
   Point d’entrée principal. Initialise toutes les données,
   lance l’algorithme sur tous les sommets (même si le graphe est déconnecté),
   collecte toutes les CFC et retourne la partition complète.
*/
t_partition find_cfcs_tarjan(t_graph graph) {
    int N = graph.num_vertices;
    t_partition partition;

    partition.num_classes = 0;
    partition.classes = NULL;
    partition.v_data = (t_tarjan_vertex *)calloc(N, sizeof(t_tarjan_vertex));

    if (partition.v_data == NULL) {
        perror("Partition v_data allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        partition.v_data[i].vertex_id = i + 1;
        partition.v_data[i].num = -1;
        partition.v_data[i].low = -1;
        partition.v_data[i].on_stack = 0;
        partition.v_data[i].class_id = 0;
    }

    current_time = 0;
    vertex_stack = create_stack(N);
    global_partition = &partition;
    global_graph = &graph;

    for (int i = 0; i < N; i++) {
        if (partition.v_data[i].num == -1) {
            tarjan_dfs(i + 1);
        }
    }

    free_stack(vertex_stack);
    global_partition = NULL;
    global_graph = NULL;

    return partition;
}

/*  
   display_partition :
   Affiche toutes les classes trouvées par Tarjan,
   leur identifiant, leur taille, et les sommets qu’elles contiennent.
*/
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

/*  
   free_partition :
   Libère proprement toute la mémoire allouée pour les CFC :
   tableau v_data, liste des classes et liste des membres de chaque classe.
*/
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
 
Si tu veux, je peux aussi te faire exactement la **même chose pour l’autre fichier (Hasse)**.
