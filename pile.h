#ifndef PILE_H
#define PILE_H

typedef struct s_cell {
    int valeur;
    struct s_cell *suivant;
} t_node;

typedef struct {
    t_node *sommet;
} t_pile;

void initPile(t_pile *p);
int estVide(t_pile *p);
void empiler(t_pile *p, int valeur);
int depiler(t_pile *p);
void afficherPile(t_pile *p);
void libererPile(t_pile *p);

#endif
