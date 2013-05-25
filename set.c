#include "set.h"

Set* init_set() {
   return NULL;
}

void empty_arcs(Arcs* e) {
   e->arcs = NULL;
}

void init_arcs(Arcs* e, int n) {
   int i; 
   empty_arcs(e);
   
   e->arcs = (int**) malloc(2 * sizeof(int*));
   e->arcs[0] = (int*) malloc(n * sizeof(int));
   e->arcs[1] = (int*) malloc(n * sizeof(int));

   for (i = 0 ; i < n ; i++)
      e->arcs[0][i] = -1;
   for (i = 0 ; i < n ; i++)
      e->arcs[1][i] = -1;
}

Set* insert_set(int v , Set *old) {
   Set *new = (Set*) malloc(sizeof(Set));
   new->vertex = v;
   new->next = old;

   return new;
}

int exist_vertex_set(int v, Set *a) {
   for ( ; a != NULL; a = a->next)
      if (a->vertex == v)
         return 1;

   return 0;
}

int saturation_set(Set* a, Arcs* M) {
   int v;
   
   for (v = a->vertex ; a != NULL ; a = a->next)
      if (M->arcs[0][v] < 0)
         return v;
   return -1;
}

Arcs* diferenca_simetrica_arcs(Arcs* M, Arcs* P) {

	return NULL;   
}

void caminho_alternante() {
  // em andamento
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
