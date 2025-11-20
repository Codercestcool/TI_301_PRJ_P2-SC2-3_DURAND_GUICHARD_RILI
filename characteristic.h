#ifndef CHARACTERISTIC_H
#define CHARACTERISTIC_H
#include "tarjan.h"

// PARTIE 3 :

//Fonction qui vérifie la transience d'une classe
int Transience(t_graph graph, t_class *classe);

//Fonction qui parcourt la chaîne et vérifie les caractéristiques
void Characterize(t_graph graph, t_partition partition);

#endif //CHARACTERISTIC_H