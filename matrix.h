#ifndef MATRIX_H
#define MATRIX_H

#include "graph.h" // Pour la conversion t_graph → t_matrix
#include "tarjan.h"

//Représente une matrice de transition (ou toute matrice NxN).
typedef struct s_matrix {
    float **data;      // Pointeur vers un tableau 2D (NxN) de flottants
    int rows;          // Nombre de lignes (N)
    int cols;          // Nombre de colonnes (N)
} t_matrix;

//Crée et initialise une matrice N x N remplie de zéros.
t_matrix create_empty_matrix(int N);

//Libère la mémoire allouée pour la matrice.
void free_matrix(t_matrix matrix);

//Copie le contenu d'une matrice source vers une matrice destination. Les deux matrices doivent avoir la même taille N x N.
void copy_matrix(t_matrix dest, t_matrix src);

//Convertit la liste d'adjacence du graphe en sa matrice de transition.
t_matrix adj_list_to_matrix(t_graph graph);

//Multiplie deux matrices carrées (A * B = C). Les matrices A et B doivent avoir la même taille N x N.
t_matrix multiply_matrices(t_matrix A, t_matrix B);

//Calcule la différence absolue cumulée entre deux matrices. Utilisé pour vérifier la convergence M^n et M^(n-1). La formule est : sum(sum(|A[i][j] - B[i][j]|))
float diff_matrices(t_matrix A, t_matrix B);



//Partie 3 etape 2 

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);
t_matrix powerMatrix(t_matrix M, int power);
t_matrix stationaryDistribution(t_matrix M);


#endif // MATRIX_H
