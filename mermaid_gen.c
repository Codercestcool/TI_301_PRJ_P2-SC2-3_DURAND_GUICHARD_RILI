#include "mermaid_gen.h"

/*  
   getID :
   Transforme un entier i en identifiant de style Mermaid pour les noeuds.
   Exemple : 1 → A, 2 → B, ..., 26 → Z, 27 → AA, etc.
   Utilise un buffer statique et retourne la chaîne correspondante.
*/
static char *getID(int i){
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Ajuste l'indice à 0-based
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Inverse la chaîne pour obtenir le bon ordre
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

/*  
   generate_mermaid_file :
   Produit un fichier texte au format Mermaid pour visualiser un graphe.
   1. Écrit les directives de configuration (layout, thème, etc.)
   2. Crée tous les sommets avec leur ID et numéro
   3. Crée toutes les arêtes avec les probabilités affichées
   Note : utilise getID pour générer les identifiants Mermaid des noeuds.
*/
void generate_mermaid_file(t_graph graph, const char *output_filename) {
    FILE *file = fopen(output_filename, "w");

    if (file == NULL) {
        perror("Could not open output file for writing");
        return;
    }

    // 1. Écriture des directives de configuration Mermaid
    fprintf(file, "--- \n");
    fprintf(file, "config:\n");
    fprintf(file, "layout: elk\n");
    fprintf(file, "theme: neo\n");
    fprintf(file, "look: neo\n");
    fprintf(file, "---\n");
    fprintf(file, "flowchart LR\n"); // Graphe orienté de gauche à droite

    // 2. Définition des sommets
    for (int i = 0; i < graph.num_vertices; i++) {
        int vertex_num = i + 1;
        char *id = getID(vertex_num);
        // Double parenthèses pour dessiner un cercle autour du numéro
        fprintf(file, "%s((%d))\n", id, vertex_num);
    }

    // 3. Définition des arêtes avec probabilités
    for (int i = 0; i < graph.num_vertices; i++) {
        int depart_num = i + 1;
        char *id_depart = getID(depart_num);

        t_edge *current = graph.adj_lists[i].head;
        while (current != NULL) {
            // Affiche l'arête avec le format Mermaid : ID_DEPART -->|PROBA|ID_ARRIVEE
            fprintf(file, "%s -->|%.2f|%s\n", id_depart, current->probability, getID(current->destination));
            current = current->next;
        }
    }

    fclose(file);
    printf("Fichier mermaid genere: %s\n", output_filename);
}
/*mermaid_gen.c génère un fichier au format Mermaid, un langage visuel permettant de représenter des graphes sous forme de schémas.
Il attribue un identifiant lisible (A, B, C, …) à chaque sommet pour faciliter l’affichage.
Il écrit la structure du graphe dans un fichier texte : chaque nœud est dessiné et chaque arête est représentée avec sa probabilité.
Il utilise un style et une mise en page prédéfinis (flowchart, orientation gauche→droite, thème “neo”).
Le résultat est un diagramme clair montrant les sommets et les transitions, facilement visualisable dans n’importe quel viewer Mermaid.
En bref : mermaid_gen.c sert à convertir le graphe en un schéma visuel automatique, pratique pour comprendre et vérifier sa structure.*/
