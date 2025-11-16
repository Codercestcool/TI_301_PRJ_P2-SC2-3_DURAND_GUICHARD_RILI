#ifndef HASSE_H
#define HASSE_H

#include "classes.h"  

// ===== Structure Lien entre classes =====
typedef struct {
    int depart;      // index classe départ  
    int arrivee;     // index classe arrivée  
} t_link;

// ===== Tableau dynamique de liens =====
typedef struct {
    t_link *links;   // tableau de liens
    int taille;      // nombre de liens utilisés
    int capacite;    // capacité actuelle du tableau
} t_link_array;


// Création / gestion du tableau de liens
t_link_array *createLinkArray(int capacite_initiale);
int linkExists(t_link_array *arr, int depart, int arrivee);
void addLink(t_link_array *arr, int depart, int arrivee);

// Construction du tableau sommet → classe
int *buildSommetToClasse(t_classe_list *classe_list, int nb_sommets);

// Construction des liens du diagramme de Hasse
void construireLiensHasse(
    int **adj, int *adjSize, int nb_sommets,
    t_classe_list *classes, t_link_array *links
);

// Affichage Mermaid
void afficherHasseMermaid(t_link_array *links);

#endif
