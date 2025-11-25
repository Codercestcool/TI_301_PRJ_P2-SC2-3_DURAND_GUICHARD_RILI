#include "markov_check.h"
#include <math.h> // Pour utiliser fabsf (valeur absolue d'un float)

//Vérifie si un graphe est un Graphe de Markov.
/*  
   is_markov_graph :
   Vérifie si un graphe orienté est un graphe de Markov.
   Pour chaque sommet, on calcule la somme des probabilités des arêtes sortantes.
   Si la somme diffère de 1 (avec une tolérance TOLERANCE), le graphe n’est pas Markovien.
   Affiche un message pour chaque sommet non conforme et retourne 1 si le graphe est Markov, 0 sinon.
*/
int is_markov_graph(t_graph graph) {
    int is_markov = 1;

    printf("--- Verification de la propriete de Markov ---\n");

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
            printf("Le graphe n'est pas un graphe de Markov.\n");
            printf("la somme des probabilites du sommet %d est %.2f (hors tolerance de +/-%.2f)\n",
                   i + 1, sum_proba, TOLERANCE);// Affichage du sommet en cause
            is_markov = 0; // Marque le graphe comme non-Markov
            // On peut choisir de continuer à vérifier les autres sommets
        }
    }

    if (is_markov) {
        // Le graphe est un graphe de Markov
        printf("Le graphe est un graphe de Markov (sommes des probabilites sortantes = 1 a + ou - %.2f pour tous les sommets).\n", TOLERANCE);
    }

    return is_markov;
}
