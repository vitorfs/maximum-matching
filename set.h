#ifndef _SET_
#define _SET_

#include <stdlib.h>
#include "graph.h"

typedef struct set {
   int vertex;
   struct set* next;
} Set;

// Semântica 1 - conjunto recíproco de arestas (se existe aresta uv, existe vu)
// Semântica 2 - caminho (existe aresta apenas em uma direção - depende de um vértice inicial)
typedef struct arcs {
   int** arcs; 
} Arcs;

// Inicializa uma lista Set vazia
// Set* X = init_set();
Set* init_set();

// limpa sujeiras da memória
void empty_arcs(Arcs* e);

// n equivale a quantidade de nós
// Índice das colunas indicam os vértices de uma extremidade da aresta
// Linha e->arcs[0][] representa o vértice da outra extremidade da aresta
// Linha e->arcs[1][] representa o peso da aresta
void init_arcs(Arcs* e, int n);

// Insere um vértice na lista Set
// X = insert_set(5, X)
Set* insert_set(int v , Set *old);

// Retorna 1 se existe e 0 se não existe o vértice na lista
int exist_vertex_set(int v, Set *a);

// Retorna -1 se o conjunto a é M-saturado ou um vértice livre maior ou igual a zero
int saturation_set(Set* a, Arcs* M);

// Retorna um conjunto de arcos que é resultado da diferença simétrica de M e P
Arcs* diferenca_simetrica_arcs(Arcs* M, Arcs* P);

// Encontra um caminho aumentante ou NULL
void caminho_aumentante();

// Da um grafo g bipartido e um vértice v, constrói dois conjuntos de vértices X e Y
void define_biparticao(int v, Graph *g, Set *X, Set *Y);

#endif
