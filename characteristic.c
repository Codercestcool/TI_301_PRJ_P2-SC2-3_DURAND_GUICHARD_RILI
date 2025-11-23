#include "characteristic.h"
#include "graph.h"
#include "tarjan.h"
#include <stdio.h>
//PARTIE 2 — étape 3 :


//Fonction qui vérifie la transience d'une classe
//Parcourir chaque sommet de la classe
//Pour chaque sommet parcourir toutes ses arêtes sortantes
//Vérifier si l'arête mène vers un sommet en dehors de la classe
//Si au moins une arête mène à l'extérieur, la classe est transitoire
//Sinon la classe est persistante (récurrente)

int Transience(t_graph graph, t_class *class){
    // Une classe est transitoire si au moins un sommet a une arête vers l'extérieur.
    for(int i = 0; i < class->num_members; i++){
        int vertex = class->members_ids[i];
        t_edge *edge = graph.adj_lists[vertex].head;
        while(edge != NULL){
            int j;
            // Vérifie si l'arête mène vers l'intérieur de la classe
            for(j = 0; j < class->num_members; j++){ // Parcours les sommets de la classe
                if(edge->destination == class->members_ids[j]){ // Compare la destination aux sommets internes à la classe
                    break; // Destination trouvée → passage
                }
            }
            if(j == class->num_members){ // Si l'arête ne correspond à aucun sommet de la classe → elle sort → transitoire
                return 1;
            }
            edge = edge->next;
        }
    }
    return 0; // Si aucune arête ne sort, la classe est récurrente / persistante
}



//Fonction qui caractérise chaque classe de la chaîne
//Doit d'abord vérifier si elle est irréductible
//La parcourir
//Sur chaque classe vérifier la transience
//Si récurrence vérifier l'absorbance de chaque sommet

void Characterize(t_graph graph, t_partition partition){
    // Vérifie si la chaîne est irréductible
    if(partition.num_classes == 1){
        printf("La chaine est irreductible\n");
    }
    else{
        printf("La chaine n'est pas irreductible\n");
    }
    for(int i = 0; i < partition.num_classes; i++){ // Parcours des classes trouvées par Tarjan
        t_class * class = &partition.classes[i];
        printf("\nClasse C%d : ", i);
        // Vérifie si la classe est transitoire
        if(Transience(graph, class) == 0){
          printf("persistante\n"); //La classe est récurrente / persistante
          if(class->num_members == 1){ //Vérifie si la classe contient un unique état
            int vertex = class->members_ids[0]; //L'unique sommet d'une classe récurrente / persistante est absorbant
            printf("L'etat %d est persistant\n", vertex);
          }
        }
        else{
            printf("transitoire\n");
        }
    }
}

// Implémentation de la fonction pour la gestion des données
void set_persistence_flags(t_graph graph, t_partition *partition) {
    for (int i = 0; i < partition->num_classes; i++) {
        // Appelle votre fonction de vérification
        if (Transience(graph, &partition->classes[i]) == 0) {
            // Si Transience retourne 0 → Persistante (Récurrente).
            partition->classes[i].is_persistent = 1;
        } else {
            // Si Transience retourne 1 → Transitoire
            partition->classes[i].is_persistent = 0;
        }
    }
}