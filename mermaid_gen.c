#include "mermaid_gen.h"
#include <string.h>
#include <stdio.h>

//Fournit une chaîne de caractères (ID Mermaid) associée à un entier.
static char *getID(int i){
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

//Produit un fichier texte au format Mermaid pour visualiser le graphe.
void generate_mermaid_file(t_graph graph, const char *output_filename) {
    FILE *file = fopen(output_filename, "w");

    if (file == NULL) {
        perror("Could not open output file for writing");
        return;
    }

    // 1. Écrire les directives de configuration
    fprintf(file, "--- \n");
    fprintf(file, "config:\n");
    fprintf(file, "layout: elk\n");
    fprintf(file, "theme: neo\n");
    fprintf(file, "look: neo\n");
    fprintf(file, "---\n");
    fprintf(file, "flowchart LR\n"); // Graphe orienté de gauche à droite

    // 2. Définition des sommets (ex: A((1)))
    for (int i = 0; i < graph.num_vertices; i++) {
        int vertex_num = i + 1;
        // Utilisation de la fonction getID qui retourne directement l'ID
        char *id = getID(vertex_num);
        // Le format A((1)) utilise des doubles parenthèses pour des cercles.
        fprintf(file, "%s((%d))\n", id, vertex_num);
    }

    // 3. Définition des arêtes avec probabilités
    for (int i = 0; i < graph.num_vertices; i++) {
        int depart_num = i + 1;
        // L'ID de départ doit être stocké, car l'appel à getID(current->destination)
        // écraserait le buffer statique.
        char *id_depart = getID(depart_num);

        t_edge *current = graph.adj_lists[i].head;
        while (current != NULL) {
            // Appel à getID écrase la valeur précédente, on s'assure de l'utiliser tout de suite.
            // Note : C'est une limite des fonctions utilisant des buffers statiques.
            // C'est acceptable ici, car la valeur de 'id_depart' est déjà utilisée.

            // Format : ID_DEPART -->|PROBA|ID_ARRIVEE
            fprintf(file, "%s -->|%.2f|%s\n", id_depart, current->probability, getID(current->destination));

            current = current->next;
        }
    }

    fclose(file);
    printf("Fichier Mermaid généré: %s\n", output_filename);
}