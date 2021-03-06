/*
 * =====================================================================================
 *
 *       Filename:  maximum-matching.c
 *
 *    Description:  Maximum Matching
 *
 *        Version:  1.0
 *        Created:  15/05/2013 10:26:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Vitor Freitas (vfs), vitorfs@gmail.com
 *                  Rodrigo Martins (rfm), drg.martins@gmail.com
 *        Company:  Universidade Federal de Juiz de Fora (UFJF)
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"


/*
 * This function has the job to print the matching result set from a graph g
 */
void print_matching(Graph* g) {
  int i, j;
  int arc_count = 0;

  printf("G = (V, A)\n");
  printf("V = {");
  for (i = 0 ; i < g->vertex_count ; i++) {
    i == 0 ? printf("%d", i) : printf(", %d", i);
  }
  printf("}\n");
  printf("A = {");
  for (i = 0 ; i < g->vertex_count ; i++) {
    for (j = i ; j < g->vertex_count ; j++) {
      if (g->arcs[i][j] > 0) {
        printf("(%d, %d)", i, j);
        arc_count++;
      }
    }
  }
  printf("}\n");

  printf("Vertex count: %d\n", g->vertex_count);
  printf("Arc count: %d\n", arc_count);
  printf("Perfect matching: ");
  arc_count == (g->vertex_count / 2) ? printf("Yes") : printf("No");
  printf("\n");
}

/*
 * This happend to be our first approuch to get the maximum matching.
 * THe results are pretty satisfatory and the code complexity isnt that bad at all
 */
Graph* maximal_matching(Graph* g) {
  Graph* m = (Graph*) malloc(sizeof(Graph));
  Graph* matching = (Graph*) malloc(sizeof(Graph));

  init_graph(m, g->vertex_count);
  init_graph(matching, g->vertex_count); 

  int i;
  int j;

  // Make a copy of the graph ignoring loops
  for (i = 0 ; i < g->vertex_count ; i++)
    for (j = 0 ; j < g->vertex_count ; j++)
      if (i != j)
        m->arcs[i][j] = g->arcs[i][j];

  Vertex* v = (Vertex*) malloc(g->vertex_count * sizeof(Vertex));
  get_ordered_vertex(m, v);
  
  int saturated[m->vertex_count];
  for (i = 0 ; i < m->vertex_count ; i++)
    saturated[i] = 0;

  for (i = 0 ; i < m->vertex_count ; i++) {
    m->vertex[i].degree = get_vertex_degree(m, i);
  }

  int* adj;
  for (i = 0 ; i < m->vertex_count ; i++) {
    int v1 = v[i].vertex;
    int v2;

    if (saturated[v1] == 0) {
      adj = get_adjacency(m, v1);
      if (adj[0] > 0) {
        v2 = adj[1];
        
        for (j = 2 ; j <= adj[0] ; j++) {
          if (m->vertex[adj[j]].degree < m->vertex[v2].degree) 
            v2 = adj[j];
        }
        
        insert_arc(matching, v1, v2, m->arcs[v1][v2]);
        free(adj);
        saturated[v1] = 1;
        saturated[v2] = 1;
        
        for (j = 0 ; j < m->vertex_count ; j++) {
          remove_arc(m, v1, j);
          remove_arc(m, v2, j);
        }
      }
    }
  }
  
  return matching;
}

