#include "matrix.h"
#include <math.h>
#include <string.h>
#include "tarjan.h"
#include "characteristic.h"

//Crée et initialise une matrice N x N remplie de zéros.
t_matrix create_empty_matrix(int N) {
    t_matrix matrix;
    matrix.rows = N;
    matrix.cols = N;
    
    // 1. Allocation des lignes (tableau de pointeurs)
    matrix.data = (float **)malloc(N * sizeof(float *));
    if (matrix.data == NULL) {
        perror("Allocation failed for matrix rows");
        exit(EXIT_FAILURE);
    }
    
    // 2. Allocation des colonnes (les données réelles) et initialisation à zéro
    for (int i = 0; i < N; i++) {
        matrix.data[i] = (float *)calloc(N, sizeof(float)); // calloc met tout à zéro
        if (matrix.data[i] == NULL) {
            perror("Allocation failed for matrix columns");
            // Libérer la mémoire déjà allouée
            for (int j = 0; j < i; j++) {
                free(matrix.data[j]);
            }
            free(matrix.data);
            exit(EXIT_FAILURE);
        }
    }
    
    return matrix;
}

//Libère la mémoire allouée pour la matrice.
void free_matrix(t_matrix matrix) {
    if (matrix.data != NULL) {
        for (int i = 0; i < matrix.rows; i++) {
            if (matrix.data[i] != NULL) {
                free(matrix.data[i]);
            }
        }
        free(matrix.data);
    }
}

//Copie le contenu d'une matrice source vers une matrice destination.
void copy_matrix(t_matrix dest, t_matrix src) {
    // Vérification basique des dimensions
    if (dest.rows != src.rows || dest.cols != src.cols) {
        fprintf(stderr, "Error: Matrices must have the same dimensions for copy.\n");
        return;
    }
    
    int N = src.rows;
    for (int i = 0; i < N; i++) {
        // memcpy peut être plus rapide que la boucle for pour copier un tableau
        memcpy(dest.data[i], src.data[i], N * sizeof(float));
    }
}

//Convertit la liste d'adjacence du graphe en sa matrice de transition. La matrice M[i][j] contient la probabilité de passer de l'état i+1 à l'état j+1.
t_matrix adj_list_to_matrix(t_graph graph) {
    int N = graph.num_vertices;
    t_matrix M = create_empty_matrix(N); // Initialise la matrice à zéro

    for (int i = 0; i < N; i++) {
        // i est l'indice du sommet de départ (i+1 est le numéro du sommet)
        t_edge *current_edge = graph.adj_lists[i].head;
        
        while (current_edge != NULL) {
            // current_edge->destination est le numéro du sommet d'arrivée (j+1)
            int j = current_edge->destination - 1; // j est l'indice du sommet d'arrivée
            float proba = current_edge->probability;

            if (j >= 0 && j < N) {
                // M[i][j] = probabilité de i+1 -> j+1
                M.data[i][j] = proba;
            } else {
                fprintf(stderr, "Warning: Edge leads to invalid vertex number %d\n", current_edge->destination);
            }

            current_edge = current_edge->next;
        }
    }
    
    return M;
}

//Multiplie deux matrices carrées (A * B = C). C[i][j] = sum(A[i][k] * B[k][j]) pour k=0 à N-1
t_matrix multiply_matrices(t_matrix A, t_matrix B) {
    int N = A.rows;
    // Vérification des dimensions (matrices carrées N x N)
    if (A.cols != N || B.rows != N || B.cols != N) {
        fprintf(stderr, "Error: Matrices must be square and matching sizes for multiplication.\n");
        exit(EXIT_FAILURE);
    }
    
    t_matrix C = create_empty_matrix(N); // Matrice résultat C (initialisée à zéro)

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0f;
            // Sommation sur la dimension k
            for (int k = 0; k < N; k++) {
                sum += A.data[i][k] * B.data[k][j];
            }
            C.data[i][j] = sum;
        }
    }
    
    return C;
}

//Calcule la différence absolue cumulée entre deux matrices.
float diff_matrices(t_matrix A, t_matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        fprintf(stderr, "Error: Matrices must have the same dimensions for diff_matrices.\n");
        return -1.0f; 
    }
    
    float total_diff = 0.0f;
    int N = A.rows;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // fabsf calcule la valeur absolue d'un float
            total_diff += fabsf(A.data[i][j] - B.data[i][j]);
        }
    }
    
    return total_diff;
}

 
//  ETAPE 2 – PARTIE 3 : Sous-matrices et distributions stationnaires
 

#include "matrix.h"
#include "tarjan.h"
#include "characteristic.h"
#include <stdlib.h>
#include <stdio.h>
 
// Sous-matrice : extrait la matrice correspondant à une classe
 
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {

    t_class class = part.classes[compo_index];
    int k = class.num_members;   // Taille de la sous-matrice

    // Création d’une matrice k×k (initialisée à zéro)
    t_matrix sub = create_empty_matrix(k);

    // Pour chaque sommet de la classe
    for (int r = 0; r < k; r++) {
        int original_row = class.members_ids[r] - 1; // Numéro de ligne dans M

        for (int c = 0; c < k; c++) {
            int original_col = class.members_ids[c] - 1; // Numéro de colonne dans M

            // Copie du coefficient correspondant
            sub.data[r][c] = matrix.data[original_row][original_col];
        }
    }

    return sub;
}

// Calcul d’une puissance de matrice : M^power
// Utilisé pour approcher la distribution stationnaire
 
t_matrix powerMatrix(t_matrix M, int power) {

    int N = M.rows;

    // Matrice résultat initialisée à l'identité
    t_matrix result = create_empty_matrix(N);
    for (int i = 0; i < N; i++)
        result.data[i][i] = 1.0f;

    // Matrice temporaire pour les multiplications
    t_matrix current = create_empty_matrix(N);
    copy_matrix(current, M);

    while (power > 0) {

        // Si bit impair → multiplier dans le résultat
        if (power % 2 == 1) {
            t_matrix temp = multiply_matrices(result, current);
            free_matrix(result);
            result = temp;
        }

        // Carré de la matrice
        t_matrix temp2 = multiply_matrices(current, current);
        free_matrix(current);
        current = temp2;

        power /= 2;
    }

    free_matrix(current);
    return result;
}
 
// Distribution stationnaire : limite de M^k
// Approché en itérant jusqu’à stabilisation
 
t_matrix stationaryDistribution(t_matrix M) {

    int N = M.rows;

    // Matrices M^(k) et M^(k-1)
    t_matrix Mk   = create_empty_matrix(N);
    t_matrix Mk_1 = create_empty_matrix(N);

    // Première approximation : M^1
    copy_matrix(Mk_1, M);

    float epsilon = 0.01f;

    // Calcul successif des puissances de M
    for (int k = 2; k <= 200; k++) {

        // Mk = Mk_1 * M
        free_matrix(Mk);
        Mk = multiply_matrices(Mk_1, M);

        // Si la différence devient faible → convergence
        float diff = diff_matrices(Mk, Mk_1);
        if (diff < epsilon) {
            free_matrix(Mk_1);
            return Mk;  // Matrice stabilisée (ligne stationnaire)
        }

        // Prépare l’itération suivante
        free_matrix(Mk_1);
        Mk_1 = create_empty_matrix(N);
        copy_matrix(Mk_1, Mk);
    }

    printf(" Avertissement : la matrice n’a pas convergé après 200 itérations.\n");
    return Mk;
}
