#ifndef MARKOV_CHECK_H
#define MARKOV_CHECK_H

#include "graph.h"

#define TOLERANCE 0.01

//Vérifie si un graphe est un Graphe de Markov.
int is_markov_graph(t_graph graph);

#endif // MARKOV_CHECK_H