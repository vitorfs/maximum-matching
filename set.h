#ifndef SET_H
#define SET_H

#include <stdlib.h>
#include "graph.h"

typedef struct set {
   int vertex;
   struct set *next;
} Set;

typedef struct header_set {
   int nodes;
   Set* first;
} HeaderSet;

// Semântica 1 - conjunto recíproco de arestas (se existe aresta uv, existe vu)
// Semântica 2 - caminho (existe aresta apenas em uma direção - depende de um vértice inicial)
typedef struct arcs {
   int n;
   int **arcs; 
} Arcs;

typedef struct {
  Set* X;
  Set* Y;
} Bipartite_Graph;

// Inicializa uma lista Set vazia
// Set* X = init_set();
Set* init_set();

// Inicializa a estrutura HeaderSet
void init_header_set(HeaderSet *head);

// Limpa sujeiras da memória
void empty_arcs(Arcs *e);

// n equivale a quantidade de nós
// Índice das colunas indicam os vértices de uma extremidade da aresta
// Linha e->arcs[0][] representa o vértice da outra extremidade da aresta
// Linha e->arcs[1][] representa o peso da aresta na semântica 1
// e serve como uma estrutura auxiliar que indica que o nó já foi visitado na semântica 2
Arcs* init_arcs(int n);

// Desaloca memória usada por Set *a
void free_set(Set *a);

// Desaloca memória usada por HeaderSet *head
void free_header_set(HeaderSet *head);

// Desaloca memória usada por Arcs *e
void free_arcs(Arcs *e);

// Coloca a estrutura com o valor padrão
void zero_header_set(HeaderSet *head);

// Colaca a estrutura com o valor padrão,
// ou seja, preenche a matriz com -1
void zero_arcs(Arcs *e);

// Insere um vértice no conjunto HeaderSet *head
// caso o mesmo já não tenha sido inserido
void insert_header_set(int v, HeaderSet *head);

// Insere uma aresta em M
void insert_arcs(int u, int v, int weight, Arcs *M);

// Remove um elemento do conjunto HeaderSet *head'
void remove_header_set(int v, HeaderSet *head);

// Remove uma aresta em M
void remove_arcs(int u, int v, Arcs *M);

// Retorna 1 se existe e 0 se não existe o vértice no conjunto
int exist_vertex_header_set(int v, HeaderSet *head);

// Retorna -1 se o conjunto a é M-saturado ou o primeiro vértice livre maior encontrado
int non_saturation_header_set(HeaderSet *head, Arcs *M);

// Retorna o primeiro vértice M-saturado ou -1 caso não tenha nenhum
int saturation_header_set(HeaderSet *head, Arcs *M);

// Constrói o conjunto NS contendo os vizinhos de S
void builds_neighborhood_header_set(HeaderSet *S, HeaderSet *NS, Graph *g);

// Retorna 1 se os conjuntos forem iguais e 0 se forem diferentes
// T sempre será subconjunto de NS
// então marca-se todos os elementos de T no vetor
// e verifica se NS contém algum elemento que não foi marcado
int compare_header_set(HeaderSet *NS, HeaderSet *T, Graph *g);

// Retorna o primeiro conjunto menos o segundo
HeaderSet* subtraction_header_set(HeaderSet *NS, HeaderSet *T, Graph *g);

// Retorna um conjunto de arcos que é resultado da diferença simétrica de M e P
// P usa da semântica 2 (caminho) e M da semântica 1 (conjunto de arcos)
void symmetric_difference_arcs(int y, Arcs *P, Arcs *M);

// Encontra um caminho aumentante ou NULL
// Retorna Arcs* da semântica 2
Arcs* augmenting_path(int u, int y, Arcs *M, Graph *g, int *visited);

// Copia o caminho aumentante de source para destination a partir do vértice y
void copy_arcs(int y, Arcs *source, Arcs *destination); 

// Imprime os vértices de Set *s
void print_set(Set* s);

// Imprime as arestas em Arcs *a
void print_arcs(Arcs* a);

// Imprime um vetor de inteiro
// Recebe como parâmetro além do vetor o tamanho do mesmo
void print_array(int size, int *array); 

// Dado um grafo g bipartido e um vértice v, constrói dois conjuntos de vértices X e Y atingíveis por v
void bipartite_define_header_set(int v, int *aux, Graph *g, HeaderSet *X, HeaderSet *Y);

// Dado um grafo g bipartido, constrói dois subconjuntos de vértices X e Y
void bipartite(Graph *g, HeaderSet *X, HeaderSet *Y);
#endif
