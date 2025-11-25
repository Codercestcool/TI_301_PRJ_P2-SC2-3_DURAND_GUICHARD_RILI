#include "matrix.h"
#include <math.h>
#include <string.h>
#include "tarjan.h"

/*  
   create_empty_matrix :
   Alloue et initialise une matrice carrée N x N remplie de zéros.
   Utilise malloc/calloc pour créer les lignes et colonnes.
*/
t_matrix create_empty_matrix(int N) {
    t_matrix matrix;
    matrix.rows = N;
    matrix.cols = N;
    
    matrix.data = (float **)malloc(N * sizeof(float *));
    if (matrix.data == NULL) {
        perror("Allocation failed for matrix rows");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < N; i++) {
        matrix.data[i] = (float *)calloc(N, sizeof(float));
        if (matrix.data[i] == NULL) {
            perror("Allocation failed for matrix columns");
            for (int j = 0; j < i; j++) free(matrix.data[j]);
            free(matrix.data);
            exit(EXIT_FAILURE);
        }
    }
    
    return matrix;
}

/*  
   free_matrix :
   Libère la mémoire allouée pour une matrice (lignes puis tableau de pointeurs).
*/
void free_matrix(t_matrix matrix) {
    if (matrix.data != NULL) {
        for (int i = 0; i < matrix.rows; i++) {
            if (matrix.data[i] != NULL) free(matrix.data[i]);
        }
        free(matrix.data);
    }
}

/*  
   copy_matrix :
   Copie les valeurs d'une matrice source vers une matrice destination.
   Les dimensions doivent correspondre.
*/
void copy_matrix(t_matrix dest, t_matrix src) {
    if (dest.rows != src.rows || dest.cols != src.cols) {
        fprintf(stderr, "Error: Matrices must have the same dimensions for copy.\n");
        return;
    }
    
    int N = src.rows;
    for (int i = 0; i < N; i++) {
        memcpy(dest.data[i], src.data[i], N * sizeof(float));
    }
}

/*  
   adj_list_to_matrix :
   Transforme la liste d'adjacence d'un graphe en matrice de transition.
   Chaque élément M[i][j] contient la probabilité de passer de i à j.
*/
t_matrix adj_list_to_matrix(t_graph graph) {
    int N = graph.num_vertices;
    t_matrix M = create_empty_matrix(N);

    for (int i = 0; i < N; i++) {
        t_edge *current_edge = graph.adj_lists[i].head;
        while (current_edge != NULL) {
            int j = current_edge->destination - 1;
            float proba = current_edge->probability;

            if (j >= 0 && j < N) {
                M.data[i][j] = proba;
            } else {
                fprintf(stderr, "Warning: Edge leads to invalid vertex number %d\n", current_edge->destination);
            }

            current_edge = current_edge->next;
        }
    }
    
    return M;
}

/*  
   multiply_matrices :
   Multiplie deux matrices carrées A et B pour produire C = A*B.
   Utilise la formule standard C[i][j] = sum(A[i][k] * B[k][j]).
*/
t_matrix multiply_matrices(t_matrix A, t_matrix B) {
    int N = A.rows;
    if (A.cols != N || B.rows != N || B.cols != N) {
        fprintf(stderr, "Error: Matrices must be square and matching sizes for multiplication.\n");
        exit(EXIT_FAILURE);
    }
    
    t_matrix C = create_empty_matrix(N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0f;
            for (int k = 0; k < N; k++) sum += A.data[i][k] * B.data[k][j];
            C.data[i][j] = sum;
        }
    }
    
    return C;
}

/*  
   diff_matrices :
   Calcule la somme des différences absolues entre deux matrices.
   Sert à vérifier la convergence lors de calcul de distribution stationnaire.
*/
float diff_matrices(t_matrix A, t_matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        fprintf(stderr, "Error: Matrices must have the same dimensions for diff_matrices.\n");
        return -1.0f; 
    }
    
    float total_diff = 0.0f;
    int N = A.rows;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            total_diff += fabsf(A.data[i][j] - B.data[i][j]);
        }
    }
    
    return total_diff;
}

// ETAPE 2 – PARTIE 3 : Sous-matrices et distributions stationnaires
/*  
   subMatrix :
   Extrait la sous-matrice correspondant à une classe (CFC) spécifique.
   Permet de travailler sur la partie de la matrice associée à cette classe.
*/
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
    t_class class = part.classes[compo_index];
    int k = class.num_members;

    t_matrix sub = create_empty_matrix(k);

    for (int r = 0; r < k; r++) {
        int original_row = class.members_ids[r] - 1;
        for (int c = 0; c < k; c++) {
            int original_col = class.members_ids[c] - 1;
            sub.data[r][c] = matrix.data[original_row][original_col];
        }
    }

    return sub;
}

/*  
   powerMatrix :
   Calcule M^power (puissance d'une matrice) par exponentiation rapide.
   Utilisé pour approcher la distribution stationnaire d'une chaîne de Markov.
*/
t_matrix powerMatrix(t_matrix M, int power) {
    int N = M.rows;

    t_matrix result = create_empty_matrix(N);
    for (int i = 0; i < N; i++) result.data[i][i] = 1.0f;

    t_matrix current = create_empty_matrix(N);
    copy_matrix(current, M);

    while (power > 0) {
        if (power % 2 == 1) {
            t_matrix temp = multiply_matrices(result, current);
            free_matrix(result);
            result = temp;
        }

        t_matrix temp2 = multiply_matrices(current, current);
        free_matrix(current);
        current = temp2;

        power /= 2;
    }

    free_matrix(current);
    return result;
}

/*  
   stationaryDistribution :
   Approxime la distribution stationnaire d'une matrice de transition.
   Multiplie successivement la matrice jusqu'à stabilisation (diff < epsilon).
   Retourne la matrice finale représentant la distribution stationnaire.
*/
t_matrix stationaryDistribution(t_matrix M) {
    int N = M.rows;

    t_matrix Mk   = create_empty_matrix(N);
    t_matrix Mk_1 = create_empty_matrix(N);
    copy_matrix(Mk_1, M);

    float epsilon = 0.01f;

    for (int k = 2; k <= 200; k++) {
        free_matrix(Mk);
        Mk = multiply_matrices(Mk_1, M);

        float diff = diff_matrices(Mk, Mk_1);
        if (diff < epsilon) {
            free_matrix(Mk_1);
            return Mk;
        }

        free_matrix(Mk_1);
        Mk_1 = create_empty_matrix(N);
        copy_matrix(Mk_1, Mk);
    }

    printf(" Avertissement : la matrice n’a pas convergé après 200 itérations.\n");
    return Mk;
}
