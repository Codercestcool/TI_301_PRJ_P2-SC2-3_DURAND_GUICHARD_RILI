#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "graph.h"
#include "tarjan.h"
#include "characteristic.h"
#include "matrix.h"

int main() {
 
    // PARTIE 1 : Tests simples  
  
    t_list *liste = createEmptyList();
    addCell(liste, 1, 0.3);
    addCell(liste, 2, 0.7);
    displayList(liste);
    printf("\n\n");

    printf("Création d'une liste d'adjacence vide de taille 3 :\n");
    t_list_adj adj = createEmptyAdjList(3);

    addCell(&(adj.tab[0]), 1, 0.5);
    addCell(&(adj.tab[0]), 2, 0.5);
    addCell(&(adj.tab[1]), 0, 1.0);
    addCell(&(adj.tab[2]), 2, 1.0);

    printf("Affichage de la liste d'adjacence :\n");
    displayListAdj(&adj);

    verifyMarkovGraph(&adj);
    exportToMermaid(&adj, "graph.mmd");

    
    // Lecture d’un graphe dans graph.txt  
 
    t_list_adj g2 = readGraph("graph.txt");
    printf("\nGraphe lu depuis graph.txt :\n");
    displayListAdj(&g2);
    verifyMarkovGraph(&g2);

     
    // CONVERSION EN STRUCTURE t_graph  
 
    t_graph graph = convertListAdj_to_graph(&g2);
 
    // PARTIE 2 
     
    t_partition partition = find_cfcs_tarjan(graph);

    printf("\n===== PARTITION TARJAN =====\n");
    display_partition(partition);
 
    // PARTIE 3 – ÉTAPE 1 : Matrice de transition
    
    printf("\n===== MATRICE DE TRANSITION =====\n");
    t_matrix M = adj_list_to_matrix(graph);

    printf("Matrice M :\n");
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.cols; j++)
            printf("%.2f ", M.data[i][j]);
        printf("\n");
    }

    
    // PARTIE 3 – ÉTAPE 2 :
    //   Distribution stationnaire par classe de Tarjan
    
    printf("\n===== ETAPE 2 : Distributions stationnaires =====\n\n");

    for (int i = 0; i < partition.num_classes; i++) {

        t_class class = partition.classes[i];
        printf("Classe C%d (taille = %d) :\n", class.id, class.num_members);

        
        // 1. Vérifier si la classe est transitoire
         
        if (Transience(graph, &class)) {
            printf(" → Classe transitoire : distribution stationnaire = [0 ... 0]\n\n");
            continue;
        }
 
        // 2. Extraire la sous-matrice associée à la classe
 
        t_matrix sub = subMatrix(M, partition, i);
        printf(" → Sous-matrice extraite (%d x %d)\n", sub.rows, sub.cols);
 
        // 3. Calculer la distribution stationnaire
        t_matrix stat = stationaryDistribution(sub);

        printf(" → Distribution stationnaire : [ ");
        for (int c = 0; c < stat.cols; c++)
            printf("%.3f ", stat.data[0][c]);
        printf("]\n\n");

        free_matrix(sub);
        free_matrix(stat);
    }

    printf("===== FIN ETAPE 2 =====\n");
 
    // LIBÉRATIONS
    free_matrix(M);
    free_partition(partition);

    return 0;
}
