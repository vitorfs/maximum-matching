#ifndef _SET_C_
#define _SET_C_

#include <stdlib.h>
#include "graph.h"

typedef struct set {
   int vertex;
   struct set* next;
} Set;

// Set* X = init_set();
Set* init_set() {
   return NULL;
}

// X = insert_set(5, X)
Set* insert_set(int v , Set *old) {
   Set *new = (Set*) malloc(sizeof(Set));
   new->vertex = v;
   new->next = old;

   return new;
}

// Retorna 1 se existe e 0 se não existe
int exist_vertex_set(int v, Set *a) {
   for ( ; a != NULL; a = a->next)
      if (a->vertex == v)
         return 1;

   return 0;
}

void saturation_set() {
}

void define_biparticao(int v, Graph *g, Set *X, Set *Y) {
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
         define_biparticao(j, g, Y, X);
}

#endif
