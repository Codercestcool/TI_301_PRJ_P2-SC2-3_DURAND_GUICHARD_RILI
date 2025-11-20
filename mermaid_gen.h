#ifndef MERMAID_GEN_H
#define MERMAID_GEN_H

#include "graph.h"

//Produit un fichier texte au format Mermaid pour visualiser le graphe.
void generate_mermaid_file(t_graph graph, const char *output_filename);

#endif // MERMAID_GEN_H