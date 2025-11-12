#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

// Initialisation
void initPile(t_pile *p) {
    p->sommet = NULL;
}

// Vérifie si la pile est vide
int estVide(t_pile *p) {
    return p->sommet == NULL;
}

// Empiler
void empiler(t_pile *p, int valeur) {
    t_node *nouveau = malloc(sizeof(Noeud));
    if (!nouveau) {
        printf("Erreur : mémoire insuffisante\n");
        exit(EXIT_FAILURE);
    }
    nouveau->valeur = valeur;
    nouveau->suivant = p->sommet;
    p->sommet = nouveau;
}

// Dépiler
int depiler(t_pile *p) {
    if (estVide(p)) {
        printf("Erreur : pile vide\n");
        exit(EXIT_FAILURE);
    }
    t_node *temp = p->sommet;
    int valeur = temp->valeur;
    p->sommet = temp->suivant;
    free(temp);
    return valeur;
}

// Afficher la pile
void afficherPile(t_pile *p) {
    t_node *courant = p->sommet;
    printf("Pile (sommet → bas) : ");
    while (courant) {
        printf("%d ", courant->valeur);
        courant = courant->suivant;
    }
    printf("\n");
}

// Libérer toute la pile
void libererPile(t_pile *p) {
    while (!estVide(p)) {
        depiler(p);
    }
}
