#ifndef CHARACTERISTIC_H
#define CHARACTERISTIC_H
#include "tarjan.h"

// PARTIE 2 étape 3 :

//Fonction qui vérifie la transience d'une classe
int Transience(t_graph graph, t_class *classe);

//Fonction qui parcourt la chaîne et vérifie les caractéristiques
void Characterize(t_graph graph, t_partition partition);

//Fonction nécessaire pour modifier la structure partition et stocker l'information de persistence (is_persistent) pour le Défi Bonus.
void set_persistence_flags(t_graph graph, t_partition *partition);

#endif //CHARACTERISTIC_H