#include "set.h"

Set* init_set() {
   return NULL;
}

void empty_arcs(Arcs *e) {
   e->arcs = NULL;
}

Arcs* init_arcs(int n) {
   int i;
   Arcs *e = (Arcs*) malloc(sizeof(Arcs));
   e->n = n;

   empty_arcs(e);

   e->arcs = (int**) malloc(2 * sizeof(int*));
   e->arcs[0] = (int*) malloc(n * sizeof(int));
   e->arcs[1] = (int*) malloc(n * sizeof(int));

   for (i = 0 ; i < n ; i++)
      e->arcs[0][i] = -1;
   for (i = 0 ; i < n ; i++)
      e->arcs[1][i] = -1;

   return e;
}

void free_set(Set *a) {
   Set *temp;

   while (a != NULL) {
      temp = a;
      a = a->next;
      free(temp);
   }
}

void free_arcs(Arcs *e) {
   free(e->arcs[0]);
   free(e->arcs[1]);
   free(e->arcs);
   empty_arcs(e);

   free(e);
}

void zero_arcs(Arcs *e) {
   int i;

   for (i = 0 ; i < e->n ; i++)
      e->arcs[0][i] = -1;
   for (i = 0 ; i < e->n ; i++)
      e->arcs[1][i] = -1;
}

Set* insert_set(int v , Set *old) {
   Set *temp = old;

   for ( ; temp != NULL ; temp = temp->next)
      if (temp->vertex == v)
         return old;

   Set *new = (Set*) malloc(sizeof(Set));
   new->vertex = v;
   new->next = old;

   return new;
}

void insert_arcs(int u, int v, int weight, Arcs *M) {
   M->arcs[0][u] = v;
   M->arcs[0][v] = u;
   M->arcs[1][u] = weight;
   M->arcs[1][v] = weight;
}

void remove_set(int v, Set *a) {
   Set *temp = init_set();

   if (a->vertex == v) {
      temp = a;
      a = a->next;
      free(temp);
   }
   for ( ; a != NULL ; a = a->next)
      if (a->next->vertex == v) {
         temp = a->next;
         a->next = a->next->next;
         free(temp);
      }
}

void remove_arcs(int u, int v, Arcs *M) {
   M->arcs[0][u] = -1;
   M->arcs[0][v] = -1;
   M->arcs[1][u] = -1;
   M->arcs[1][v] = -1;
}

int exist_vertex_set(int v, Set *a) {
   for ( ; a != NULL; a = a->next)
      if (a->vertex == v)
         return 1;

   return 0;
}

int non_saturation_set(Set *a, Arcs *M) {
   int v;
   
   for ( ; a != NULL ; a = a->next) {
      v = a->vertex;
      if (M->arcs[0][v] < 0)
         return v;
   }
   return -1;
}

int saturation_set(Set *a, Arcs *M) {
   int v;

   for ( ; a != NULL ; a = a->next) {
      v = a->vertex;
      if (M->arcs[0][v] >= 0)
         return v;
   }
   return -1;
}

void builds_neighborhood_set(Set *S, Set *NS, Graph *g) {
   int v, j;
   
   for ( ; S != NULL ; S = S->next) {
      v = S->vertex;
      for (j = 0 ; j < g->vertex_count ; j++) {
         if (g->arcs[v][j] > 0)
            insert_set(j, NS);
      }
   }
}

// Se número de elementos em NS e T forem diferente já descarta
int compare_set(Set *NS, Set *T, Graph *g) {
   int *t = (int*) calloc(g->vertex_count * sizeof(int));

   for ( ; T != NULL ; T = T->next)
      t[NS->vertex] = 1;

   for ( ; NS != NULL ; NS = NS->next)
      if(!t[T->vertex])
         return 0;

   return 1;
}

void symmetric_difference_arcs(int y, Arcs *P, Arcs *M) {
   while (y >= 0) {
      if (M->arcs[0][y] != P->arcs[0][y])
         insert_arcs(y, P->arcs[0][y], P->arcs[1][y], M);
      else
         remove_arcs(y, P->arcs[0][y], M);
      y = P->arcs[0][y];
   }
}

void bipartite_define_set(int v, Graph *g, Set *X, Set *Y) {
   int i, j;
   // Estrutura para auxiliar a busca por profundidade
   // Evita que vértices sejam inseridos mais de uma vez
   // Otimiza o processo se comparada com o uso da função exist_vertex_set()
   int aux[g->vertex_count];

   for (i = 0 ; i < g->vertex_count ; i++)
      aux[i] = 0;
  
   insert_set(v, X);
   aux[v] = 1;
  
   for (j = 0 ; j < g->vertex_count ; j++)
      if (g->arcs[v][j] > 0 && aux[v] == 0)
         bipartite_define_set(j, g, Y, X);
}
