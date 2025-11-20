#include "characterteristic.h"
#include "graph.h"
#include "tarjan.h"
#include <stdio.h>
//PARTIE 2 — étape 3 :


//Fonction qui vérifie la transience d'une classe
//Parcourir chaque sommet de la classe
//Pour chaque sommet parcourir toutes ses arêtes sortantes
//Vérifier si l'arête mène vers un sommet en dehors de la classe
//Si au moins une arête mène à l'extérieur, la classe est transitoire
//Sinon la classe est persistante (réccurente)

int Transience(t_graph graph, t_class *classe){
    // Une classe est transitoire si au moins un sommet a une arête vers l'extérieur
    for(int i = 0; i < classe->num_members; i++){
        int vertex = classe->members_ids[i];
        t_edge *edge = graph.adj_lists[vertex].head;
        while(edge != NULL){
            int j;
            // Vérifie si l'arête mène vers l'intérieur de la classe
            for(j = 0; j < classe->num_members; j++){ // Parcours les sommets de la classe
                if(edge->destination == classe->members_ids[j]){ // Compare la destination aux sommets internes à la classe
                    break; // Destination trouvé -> passage
                }
            }
            if(j == classe->num_members){ // Si l'arête ne correspond à aucun sommet de la classe => elle sort → transitoire
                return 1;
            }
            edge = edge->next;
        }
    }
    return 0; // Si aucune arête ne sort, la classe est réccurente / persistante
}



//Fonction qui caractérise chaques classes de la chaîne
//Doit d'abord vérifier si elle est irréductible
//La parcourir
//Sur chaque classe vérifier la transience
//Si réccurence vérifier l'absorbance de chaques sommets

void Characterize(t_graph graph, t_partition partition){
    // Vérifie si la chaîne est irréductible
    if(partition.num_classes == 1){
        printf("La chaîne est irréductible\n");
    }
    else{
        printf("La chaîne n'est pas irréductible\n");
    }
    for(int i = 0; i < partition.num_classes; i++){ // Parcours des classes trouvées par Tarjan
        t_class * classe = &partition.classes[i];
        printf("\nClasse C%d : ", i);
        // Vérifie si la classe est transitoire
        if(Transience(graph, classe) == 0){
          printf("persistante\n"); //La classe est récurrente / persistante
          if(classe->num_members == 1){ //Vérifie si la classe contient un unique état
            int vertex = classe->members_ids[0]; //L'unique sommet d'une classe réccurente / persistente est absorbant
            printf("L'état %d est persistant\n", vertex);
          }
        }
        else{
            printf("transitoire\n");
        }
    }
}

