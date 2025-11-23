# TI301 - Analyse de Graphes de Markov

## 📝 Description du Projet

Ce projet C implémente une analyse complète des **Graphes de Markov** (ou Chaînes de Markov à temps discret) basés sur une matrice de transition.

Le programme réalise les étapes d'analyse suivantes :
1.  **Lecture** du graphe à partir d'un fichier texte et vérification de la **propriété de Markov**.
2.  **Analyse structurelle** : Identification des **Classes** (Composantes Fortement Connexes) via l'algorithme de Tarjan.
3.  Détermination des classes **Persistantes** et **Transitoires**, et du **Diagramme de Hasse**.
4.  **Calcul de la distribution stationnaire** ($\lim M^k$) par convergence.
5.  (Défi Bonus) Calcul de la **Période** de chaque classe persistante.

Toutes les visualisations sont générées au format **Mermaid** pour un affichage facile via un outil de rendu graphique externe.

---

## ⚙️ Compilation et Structure

### Structure des Fichiers

| Fichier C | Fichier H | Rôle Principal |
| :--- | :--- | :--- |
| `main.c` | - | Orchestration, gestion des entrées/sorties. |
| `graph.c` | `graph.h` | Gestion de la liste d'adjacence et lecture des données. |
| `markov_check.c` | `markov_check.h` | Vérification de la contrainte de Markov. |
| `tarjan.c` | `tarjan.h` | Implémentation de l'algorithme de Tarjan (Classes/CFCs). |
| `hasse.c` | `hasse.h` | Analyse des types de classes et construction du Diagramme de Hasse. |
| `matrix.c` | `matrix.h` | Fonctions matricielles et distribution stationnaire. |
| `period.c` | `period.h` | Défi Bonus : Calcul du PGCD et de la période. |
| `mermaid_gen.c` | `mermaid_gen.h` | Génération des fichiers de visualisation Mermaid. |
| **`data/`** | - | **Dossier contenant tous les fichiers d'exemples d'entrée.** |
| **`CMakeLists.txt`** | - | **Fichier de configuration pour CLion/CMake.** |

### Compilation (exclusivement via CLion/CMake)

Le projet utilise **CMake** pour la gestion de la compilation et des dépendances.

1.  Assurez-vous que le fichier **`CMakeLists.txt`** est à la racine de votre projet.
2.  Ouvrez le dossier du projet dans **CLion**. L'EDI détectera automatiquement la configuration CMake, gérera la compilation de tous les fichiers sources (`.c`) et liera la librairie mathématique (`-lm`).
3.  Utilisez le bouton **"Build"** (marteau) de CLion pour compiler l'exécutable **`markov_analyzer`**.

---

## 🚀 Utilisation du Programme

### Exécution

L'exécutable `markov_analyzer` se lance en spécifiant (ou en laissant l'utilisateur choisir) le nom du fichier à analyser. Ce fichier doit se trouver dans le dossier **`data/`**.

**Dans le terminal (depuis le dossier de l'exécutable) :**

```bash
# Exemple 1: Analyser le fichier 'data/mon_graphe.txt'
./markov_analyzer mon_graphe.txt

# Exemple 2: Lancer sans argument. Le programme demandera le nom du fichier.
./markov_analyzer
