#include "markov_check.h"
#include <math.h> // Pour utiliser fabsf (valeur absolue d'un float)
#include <stdio.h>
#include <stdlib.h>

//Vérifie si un graphe est un Graphe de Markov.
int is_markov_graph(t_graph graph) {
    int is_markov = 1;

    printf("--- Vérification de la Propriété de Markov ---\n");

    // Parcourir tous les sommets (de 1 à N)
    for (int i = 0; i < graph.num_vertices; i++) {
        float sum_proba = 0.0f;
        
        // Parcourir toutes les arêtes sortantes du sommet i+1
        t_edge *current = graph.adj_lists[i].head;
        while (current != NULL) {
            sum_proba += current->probability;
            current = current->next;
        }

        // Vérification avec une tolérance
        if (fabsf(sum_proba - 1.0f) > TOLERANCE) {
            // Le graphe n’est pas un graphe de Markov
            printf("Le graphe n'est PAS un graphe de Markov.\n");
            printf("la somme des probabilités du sommet %d est %.2f (hors tolérance de +/-%.2f)\n", 
                   i + 1, sum_proba, TOLERANCE);// Affichage du sommet en cause [cite: 117, 118]
            is_markov = 0; // Marque le graphe comme non-Markov
            // On peut choisir de continuer à vérifier les autres sommets
        }
    }

    if (is_markov) {
        // Le graphe est un graphe de Markov
        printf("Le graphe est un graphe de Markov (sommes des probabilités sortantes = 1 ± %.2f pour tous les sommets).\n", TOLERANCE);
    }
    printf("------------------------------------------------\n");

    return is_markov;
}