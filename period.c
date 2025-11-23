#include "period.h"
#include <stdio.h>
#include <stdlib.h>

//Calcule le Plus Grand Commun Diviseur (PGCD) de deux nombres entiers.
static int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

//Calcule le Plus Grand Commun Diviseur (PGCD) d'un tableau d'entiers.
int gcd_array(int *vals, int nb_vals) {
    if (nb_vals == 0) return 0; // Aucun PGCD si tableau vide
    
    int result = vals[0];
    
    for (int i = 1; i < nb_vals; i++) {
        // Le nouveau PGCD est le PGCD du résultat actuel et du prochain nombre
        result = gcd(result, vals[i]);
    }
    
    return result;
}

//Calcule la période d'une classe à partir de sa sous-matrice de transition. Le principe est de trouver l'ensemble K des k >= 1 pour lesquels (M^k)[i][i] > 0 pour n'importe quel sommet i de la classe. La période est alors PGCD(K).
int get_class_period(t_matrix sub_matrix) {
    
    int n = sub_matrix.rows;
    // Période toujours 1 pour un état ou une classe singleton
    if (n <= 1) return 1; 

    // periods stockera les longueurs de chemins de retour (k)
    // On alloue de l'espace pour une limite de n*n
    int *periods = (int *)malloc(n * n * sizeof(int)); 
    if (!periods) {
        perror("Memory allocation failed for periods array");
        return -1;
    }
    
    int period_count = 0;
    
    // current_power = M^cpt
    t_matrix current_power = create_empty_matrix(n); 
    // Initialisation à M^1 (M)
    copy_matrix(current_power, sub_matrix); 

    // Limite de recherche : 2*n est souvent suffisant pour des classes de petite taille.
    // Cette limite évite une boucle infinie.
    int max_iter = 2 * n; 
    
    for (int cpt = 1; cpt <= max_iter; cpt++) {

        // --- 1. Vérification de la diagonale ---
        // S'il existe un i tel que (M^cpt)[i][i] > 0, c'est un chemin de retour de longueur cpt
        int diag_nonzero = 0;
        for (int i = 0; i < n; i++) {
            if (current_power.data[i][i] > 0.0f) {
                diag_nonzero = 1;
                break; 
            }
        }

        if (diag_nonzero) {
            // Ajouter cpt à la liste des longueurs de retour
            if (period_count < n * n) { 
                periods[period_count++] = cpt;
            }
        }
        
        // --- 2. Arrêt et PGCD ---
        // Si plus d'une période est trouvée, on peut vérifier la convergence du PGCD
        // Mais pour simplifier, on calcule la puissance suivante jusqu'à max_iter.
        
        // --- 3. Calcul de la puissance suivante : M^(cpt+1) = M^cpt * M ---
        if (cpt < max_iter) {
            t_matrix next_power = multiply_matrices(current_power, sub_matrix);
            free_matrix(current_power);
            current_power = next_power;
        }
    }

    // Libération de la mémoire
    free_matrix(current_power);
    
    int period;
    if (period_count == 0) {
        // Cas d'erreur ou limite max_iter trop petite
        period = 0; 
    } else {
        // La période est le PGCD de toutes les longueurs de chemins de retour trouvées.
        period = gcd_array(periods, period_count);
    }
    
    free(periods);
    return period;
}