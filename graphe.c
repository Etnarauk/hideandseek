/* effets :
- 0 : vide
- 1 : malus
- 2 : bonus
- 3 : or
- 4 : boutique*/

/* ordre tableau d'adjacence :
0 : gauche
1 : droite
2 : haut
3 : bas
4 : gauche_bas
5 : droite_bas
6 : gauche_haut
7 : droite_haut
*/



/* LIBRAIRIES */





#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>



/* STRUCTURES */




struct sommet_s{
  int numero;
  int effet;
  int* tab_adj;
};
typedef struct sommet_s sommet;

struct graphe_s{
    int taille;
    sommet** sommets;
};
typedef struct graphe_s graphe;

struct objet_s{
    int id;
    int nb;
};
typedef struct objet_s objet;

struct joueur_s{
    int numero;
    int nb_or;
    int position;
    bool chercheur;
    int taille_inventaire;
    objet* inventaire;
};
typedef struct joueur_s joueur;





/* JOUEURS */





void change_roles(joueur j1, joueur j2, int n){
    if (j1.chercheur){
        j1.chercheur = false;
        j2.position = rand() % n;
        j2.chercheur = true;
    }
}

bool meme_objet(objet o1, objet o2){
    return (o1.id == o2.id);
}

int possede(joueur j, objet o){
    int indice = -1;
    for (int i = 0 ; i < 4 ; i++){
        if (meme_objet(j.inventaire[i], o)){
            indice = i;
        }
    }
    return indice;
}

void met_objet_inventaire(joueur j, objet o){
    int i = possede(j, o);
    if (i == -1){
        if (j.taille_inventaire < 3){
            j.inventaire[j.taille_inventaire] = o;
            j.taille_inventaire = j.taille_inventaire + 1;
        }
    }
    else{objet ob = j.inventaire[i];
    ob.nb = ob.nb + 1;
    j.inventaire[i] = ob;}
}

void effet0(joueur j1, joueur j2){
    j1.nb_or = j1.nb_or + 5;
    j2.nb_or = j2.nb_or - 5;
}




/* GRAPHE */




int suivant(int i, int j){
    if (i == j){return 0;} else {return i + 1;}
}

graphe* initialise_graphe(int n){
    graphe* g = malloc(sizeof(graphe));
    g->taille = n;
    g->sommets = malloc(sizeof(sommet)*n);
    for (int i = 0 ; i < n ; i++){
        sommet *s = malloc(sizeof(sommet));
        s->numero = i;
        s->effet = 0;
        s->tab_adj = malloc(sizeof(int)*8);
        g->sommets[i] = s;
    }
    for (int i = 0 ; i < n ; i++){
        for (int j = 0 ; j < 8 ; j++){
            g->sommets[i]->tab_adj[j] = (rand() % (n + 2)) - 2;
            g->sommets[i]->tab_adj[rand() % 8] = suivant(i, n - 1);
        }
    }
    return g;
}

bool est_possible(graphe *g, int nb_malus, int nb_bonus, int nb_or, int nb_boutique){
    return (g->taille > nb_malus + nb_bonus + nb_or + nb_boutique);
}

void met_cases_graphe(graphe *g, int nb_malus, int nb_bonus, int nb_or, int nb_boutique){
    if (est_possible(g, nb_malus, nb_bonus, nb_or, nb_boutique)){
    int i = 0;
    while (i < nb_malus){
        g->sommets[i]->effet = 1;
        i = i + 1;
    }
    i = 0;
    while (i < nb_bonus){
        g->sommets[nb_malus + i]->effet = 2;
        i = i + 1;
    }
    i = 0 ;
    while (i < nb_or){
        g->sommets[nb_malus + nb_bonus + i]->effet = 3;
        i = i + 1;
    }
    i = 0 ;
    while (i < nb_boutique){
        g->sommets[nb_malus + nb_bonus + nb_or + i]->effet = 4;
        i = i + 1;
    }
    }
}

int* permutation(int n){
    bool *deja_utilise = malloc(sizeof(bool)*n);
    for (int i = 0 ; i < n ; i++){
        deja_utilise[i] = false;
    }
    int* permute = malloc(sizeof(int)*n);
    for (int i = 0 ; i < n ; i++){
        int a = rand() % n;
        while (deja_utilise[a] == true){
            a = a = rand() % n;
        }
        permute[i] = a;
        deja_utilise[a] = true;
    }
    return permute;
}

void permute_sommets(graphe *g){
    int* permute = permutation(g->taille);
    for (int i = 0 ; i < g->taille ; i++){
        g->sommets[i]->numero = permute[i];
    }
}

void print_numeros(graphe *g){
    for (int i = 0 ; i < g->taille ; i++){
        printf("%d : %d\n", i, g->sommets[i]->numero);
    }
}

void print_voisins(graphe *g){
    for (int i = 0 ; i < g->taille ; i++){
        printf("%d :\n", i);
        printf("voisin gauche : %d\n", g->sommets[i]->tab_adj[0]);
        printf("voisin droite : %d\n", g->sommets[i]->tab_adj[1]);
        printf("voisin haut : %d\n", g->sommets[i]->tab_adj[2]);
        printf("voisin bas : %d\n", g->sommets[i]->tab_adj[3]);
        printf("voisin gauche_bas : %d\n", g->sommets[i]->tab_adj[4]);
        printf("voisin droite_bas : %d\n", g->sommets[i]->tab_adj[5]);
        printf("voisin gauche_haut : %d\n", g->sommets[i]->tab_adj[6]);
        printf("voisin droite_bas : %d\n", g->sommets[i]->tab_adj[7]);
        printf("----\n");
        printf("\n");
    }
}

void print_tab(int *tab, int n){
    for (int i = 0 ; i < n ; i++){
        printf("%d : %d\n", i, tab[i]);
    }
}

void produit_graphe(int n, int nb_malus, int nb_bonus, int nb_or, int nb_boutique){
    graphe* g = initialise_graphe(n);
    met_cases_graphe(g, nb_malus, nb_bonus, nb_or, nb_boutique);
    permute_sommets(g);
}

int main(){
    srand(time(NULL));
    return 0;
}

