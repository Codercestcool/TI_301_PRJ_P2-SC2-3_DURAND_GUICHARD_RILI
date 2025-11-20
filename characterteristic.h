#ifndef CHARACTERTERISTC_H
#define CHARACTERTERISTIC_H
#include "tarjan.h"

#endif //CHARACTERTERISTIC_H

// PARTIE 3 :

//fonction qui vérifie la transience d'une classe
int Transience(t_graph graph, t_class *classe);

//fonction qui parcours la chaîne et vérifie les caractéristiques
void Characterize(t_graph graph, t_partition partition);