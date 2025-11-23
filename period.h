#ifndef PERIOD_H
#define PERIOD_H

#include "matrix.h" // Pour t_matrix
#include "tarjan.h"

// Calcule le Plus Grand Commun Diviseur (PGCD) d'un tableau d'entiers.
int gcd_array(int *vals, int nb_vals);

//Calcule la période d'une classe (CFC) à partir de sa sous-matrice de transition. La période est le PGCD des longueurs de chemins pour revenir à n'importe quel sommet.
int get_class_period(t_matrix sub_matrix);

#endif // PERIOD_H