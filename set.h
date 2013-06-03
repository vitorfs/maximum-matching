#ifndef _SET_
#define _SET_

#include <stdlib.h>
#include "graph.h"

typedef struct set {
   int vertex;
   struct set *next;
} Set;

// Semântica 1 - conjunto recíproco de arestas (se existe aresta uv, existe vu)
// Semântica 2 - caminho (existe aresta apenas em uma direção - depende de um vértice inicial)
typedef struct arcs {
   int n;
   int **arcs; 
} Arcs;

// Inicializa uma lista Set vazia
// Set* X = init_set();
Set* init_set();

// Limpa sujeiras da memória
void empty_arcs(Arcs *e);

// n equivale a quantidade de nós
// Índice das colunas indicam os vértices de uma extremidade da aresta
// Linha e->arcs[0][] representa o vértice da outra extremidade da aresta
// Linha e->arcs[1][] representa o peso da aresta
Arcs* init_arcs(int n);

// Desaloca memória usada por Set *a
void free_set(Set *a);

// Desaloca memória usada por Arcs *e
void free_arcs(Arcs *e);

// Colaca a estrutura com o valor padrão,
// ou seja, preenche a matriz com -1
void zero_arcs(Arcs *e);

// Insere um vértice na lista Set
// caso o mesmo já não tenha sido inserido
Set* insert_set(int v , Set *old);

// Insere uma aresta em M
void insert_arcs(int u, int v, int weight, Arcs *M);

// Remove um elemento do conjunto Set *a
void remove_set(int v, Set *a);

// Remove uma aresta em M
void remove_arcs(int u, int v, Arcs *M);

// Retorna 1 se existe e 0 se não existe o vértice na lista
int exist_vertex_set(int v, Set *a);

// Retorna -1 se o conjunto a é M-saturado ou um vértice livre maior ou igual a zero
int non_saturation_set(Set *a, Arcs *M);

// Retorna o primeiro vértice M-saturado ou -1 caso não tenha nenhum
int saturation_set(Set *a, Arcs *M);

// Constrói o conjunto NS contendo os vizinhos de S
void builds_neighborhood_set(Set *S, Set *NS, Graph *g);

// Retorna 1 se os conjuntos forem iguais e 0 se forem diferentes
// NS sempre será maior que T
// então marca-se todos os elementos de T no vetor
// e verifica se NS contém algum elemento que não foi marcado
int compare_set(Set *NS, Set *T, Graph *g);

// Retorna o primeiro conjunto menos o segundo
Set* subtraction_set(Set *NS, Set *T, Graph *g);

// Retorna um conjunto de arcos que é resultado da diferença simétrica de M e P
// P usa da semântica 2 (caminho) e M da semântica 1 (conjunto de arcos)
void symmetric_difference_arcs(int y, Arcs *P, Arcs *M);

// Encontra um caminho aumentante ou NULL
Arcs* augmenting_path(int u, int y, Arcs *M, Graph *g);

// Dado um grafo g bipartido e um vértice v, constrói dois conjuntos de vértices X e Y
void bipartite_define_set(int v, Graph *g, Set *X, Set *Y);

#endif
