#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "graph.h"
#include "markov_check.h"
#include "mermaid_gen.h"
#include "tarjan.h"
#include "hasse.h"
#include "characteristic.h"
#include "matrix.h"
#include "period.h"

#define DATA_FOLDER "../data/"
#define DEFAULT_INPUT_FILE "exemple1.txt"
#define MAX_PATH_LENGTH 256

//Affiche les caractéristiques d'irréductibilité et les états absorbants.
void display_graph_characteristics(t_graph graph, t_partition partition);

//Affiche le vecteur de distribution stationnaire (première ligne de la matrice limite).
static void display_stationary_distribution(t_matrix matrix_limit);


int main(int argc, char *argv[]) {
    // --- Déclarations des structures principales ---
    t_graph graph;
    t_partition partition;
    t_link_array *hasse_links = NULL;
    t_matrix matrix_T;
    t_matrix matrix_limit = {0};

    // Chemins et noms de fichiers
    char full_input_path[MAX_PATH_LENGTH];
    char output_graph_path[MAX_PATH_LENGTH];
    char output_hasse_path[MAX_PATH_LENGTH];
    char base_name[MAX_PATH_LENGTH] = {0};
    char user_input[MAX_PATH_LENGTH] = {0};
    const char *input_filename = DEFAULT_INPUT_FILE;

    // --- 1. Détermination du fichier d'entrée ---
    if (argc > 1) {
        input_filename = argv[1];
    } else {
        // Demander à l'utilisateur quel fichier analyser
        printf("Entrez le nom du fichier à analyser dans le dossier data/ (par defaut : %s): ", DEFAULT_INPUT_FILE);
        if (scanf("%s", user_input) == 1 && user_input[0] != '\0') {
             input_filename = user_input;
        }
    }

    // a) Construction du chemin complet : "data/fichier_specifie.txt"
    if (snprintf(full_input_path, MAX_PATH_LENGTH, "%s%s", DATA_FOLDER, input_filename) >= MAX_PATH_LENGTH) {
        fprintf(stderr, "Erreur: Chemin de fichier trop long.\n");
        return EXIT_FAILURE;
    }

    // b) Construction des noms de fichiers de sortie basés sur le nom d'entrée

    // Copie pour manipulation car strrchr et strtok modifient la chaîne
    char temp_name[MAX_PATH_LENGTH];
    strncpy(temp_name, input_filename, MAX_PATH_LENGTH - 1);
    temp_name[MAX_PATH_LENGTH - 1] = '\0';

    char *dot = strrchr(temp_name, '.'); // Trouve le dernier point
    if (dot) {
        *dot = '\0'; // Coupe la chaîne pour obtenir le nom de base
    }
    strncpy(base_name, temp_name, MAX_PATH_LENGTH);

    // Exemple : file1.txt → file1_graph.mmd
    snprintf(output_graph_path, MAX_PATH_LENGTH, "%s_graph.mmd", base_name);
    // Exemple : file1.txt → file1_hasse.mmd
    snprintf(output_hasse_path, MAX_PATH_LENGTH, "%s_hasse.mmd", base_name);


    printf("==============================================\n");
    printf("         Analyse de graphes de Markov         \n");
    printf("==============================================\n");
    printf("Tentative de lecture du fichier : %s\n", full_input_path);

    // ============================================
    // PARTIE 1 : LECTURE ET VÉRIFICATION DU GRAPHE
    // ============================================
    printf("\n--- PARTIE 1 : Initialisation et verification ---\n");

    // 1.1 Lecture du Graphe
    graph = read_graph(full_input_path);
    if (graph.num_vertices == 0) {
        fprintf(stderr, "Erreur: Lecture du graphe echouee. Verifiez le chemin ou le format du fichier.\n");
        return EXIT_FAILURE;
    }
    printf("\nGraphe lu avec %d sommets.\n\n", graph.num_vertices);

    // 1.2 Vérification de la Propriété de Markov
    if (!is_markov_graph(graph)) {
        printf("Verification echouee : Ce n'est pas un graphe de Markov valide (somme des probabilites != 1).\n");
        free_graph(graph);
        return EXIT_FAILURE;
    }
    printf("Le graphe est valide pour l'etude de Markov.\n\n");

    // 1.3 Génération de la Visualisation du Graphe (Mermaid)
    generate_mermaid_file(graph, output_graph_path);
    printf("\n => Graphe visualise dans : %s\n", output_graph_path);


    // ====================================================
    // PARTIE 2 : ANALYSE DES CLASSES ET DIAGRAMME DE HASSE
    // ====================================================
    printf("\n--- PARTIE 2 : Structure du graphe ---\n");

    // 2.1 Algorithme de Tarjan : Trouver les CFCs (Classes)
    printf("\n2.1 Recherche des classes (CFCs) via Tarjan\n\n");
    partition = find_cfcs_tarjan(graph);

    // 2.2 Détermination des types de Classes (Stockage de l'information)
    printf("\n2.2 Stockage de la persistance pour l'analyse future\n\n");
    // Fonction nécessaire pour mettre à jour le champ is_persistent de la structure partition.
    set_persistence_flags(graph, &partition);

    // 2.3 Affichage des caractéristiques (Utilisation de votre fonction Characterize)
    Characterize(graph, partition);

    // 2.4 Construction du Diagramme de Hasse (Graphe des Classes)
    printf("\n2.4 Construction du diagramme de Hasse\n\n");
    hasse_links = compute_hasse_diagram_links(graph, partition);

    // 2.5 Visualisation du Diagramme de Hasse (Mermaid)
    if (hasse_links) {
        generate_hasse_mermaid_file(hasse_links, output_hasse_path, partition);
        printf("\n => Diagramme de Hasse visualise dans : %s\n", output_hasse_path);
    }


    // ================================
    // PARTIE 3 : CALCULS PROBABILISTES
    // ================================
    printf("\n--- PARTIE 3 : Probabilites et convergence ---\n");

    // 3.1 Conversion en Matrice de Transition (T)
    matrix_T = adj_list_to_matrix(graph);

    // 3.2 Calcul de la Distribution Stationnaire (Lim T^k)
    printf("\n3.2 Calcul de la distribution stationnaire (tolerance 0.01)...\n\n");
    // Utilisation du nom de fonction requis
    matrix_limit = stationaryDistribution(matrix_T);

    // 3.3 Affichage de la Distribution Limite
    display_stationary_distribution(matrix_limit);

    // =================================
    // DÉFI BONUS : CALCUL DE LA PÉRIODE
    // =================================
    printf("\n--- DEFI BONUS : Calcul de la periode ---\n");

    int found_persistent_class = 0;

    if (partition.num_classes > 0) {
        for (int i = 0; i < partition.num_classes; i++) {
            t_class current_class = partition.classes[i];

            if (current_class.is_persistent) {
                found_persistent_class = 1;

                t_matrix sub_M = subMatrix(matrix_T, partition, i);

                int period = get_class_period(sub_M);

                printf("Classe C%d (%s) : Periode = %d\n",
                       current_class.id,
                       period == 1 ? "Aperiodique" : "Periodique",
                       period);

                free_matrix(sub_M);
            }
        }

        if (!found_persistent_class) {
            printf("Statut : Aucune classe persistante trouvee. Periode non calculee.\n");
        }

    } else {
        printf("Statut : Aucune classe trouvee. Analyse de periode impossible.\n");
    }


    // ================
    // NETTOYAGE ET FIN
    // ================

    free_matrix(matrix_T);
    if (matrix_limit.data != NULL) free_matrix(matrix_limit);
    free_link_array(hasse_links);
    free_partition(partition);
    free_graph(graph);

    return EXIT_SUCCESS;
}

//Affiche les caractéristiques d'irréductibilité et les états absorbants.
void display_graph_characteristics(t_graph graph, t_partition partition) {
    printf("\n--- Caracteristiques du graphe ---\n");

    // 1. Irréductibilité
    if (partition.num_classes == 1) {
        printf("Propriete : Le graphe est irreductible (une seule classe).\n");
    } else {
        printf("Propriete : Le graphe est reductible (%d classes).\n", partition.num_classes);
    }

    // 2. États Absorbants (Classe Persistante de taille 1)
    int has_absorbing = 0;
    for (int i = 0; i < partition.num_classes; i++) {
        t_class c = partition.classes[i];

        if (c.is_persistent && c.num_members == 1) {
            printf("Etat absorbant trouve : sommet %d.\n", c.members_ids[0]);
            has_absorbing = 1;
        }
    }
    if (!has_absorbing) {
        printf("Aucun etat absorbant trouve.\n");
    }
}


//Affiche le vecteur de distribution stationnaire (première ligne de la matrice limite).
static void display_stationary_distribution(t_matrix matrix_limit) {
    if (matrix_limit.rows == 0 || matrix_limit.data == NULL) {
        printf("Distribution stationnaire non calculee ou non convergee.\n");
        return;
    }

    printf("Vecteur de distribution stationnaire (Lim M^k):\n\n");
    printf("Sommet | Probabilite\n");
    printf("-------------------\n");

    // La distribution stationnaire est la première ligne (et toutes les autres) de la matrice limite
    for (int i = 0; i < matrix_limit.cols; i++) {
        printf("  %02d    |   %.4f\n", i + 1, matrix_limit.data[0][i]);
    }
}