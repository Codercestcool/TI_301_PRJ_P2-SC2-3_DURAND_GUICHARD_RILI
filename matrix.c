#include "matrix.h"
#include <math.h>

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