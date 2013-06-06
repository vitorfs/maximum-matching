#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "set.h"

int PRINT_MATRIX = 0;

// Recebe um grafo g
// Retorna um emparelhamento maximal
// Deve ser reexecutado para todo vértice não M-saturado
Arcs* hungarian(Graph *g) {
   int i, j, u;
   Set *S, *T, *NS, *X, *Y;
   Arcs *M;

   X = init_set();
   Y = init_set();
   //bipartite_define_set(0, g, X, Y);

   S = init_set();
   T = init_set();
   NS = init_set();

   M = init_arcs(g->vertex_count);
   for (i = 0 ; i < g->vertex_count ; i++)
      for (j = i ; j < g->vertex_count ; j++)
         if (g->arcs[i][j] > 0)
            insert_arcs(i, j, g->arcs[i][j], M);

   u = non_saturation_set(X, M);
   while (u >= 0) {

      int y;

      insert_set(u, S);
      free_set(T);
      free_set(NS);
      builds_neighborhood_set(S, NS, g);

      while (!compare_set(NS, T, g)) {
         y = saturation_set(subtraction_set(NS, T, g), M);
         if (y < 0) break;

         insert_set(M->arcs[0][y], S);
         insert_set(y, T);

         builds_neighborhood_set(S, NS, g);
      }
      if (compare_set(NS, T, g))
         return M;
         //return S;
      else {
         Arcs* P;
         P = init_arcs(g->vertex_count);
         y = non_saturation_set(subtraction_set(NS, T, g), M);

         P = augmenting_path(u, y, M, g);
         symmetric_difference_arcs(y, P, M);
      }

      u = non_saturation_set(X, M);
   }

   return M;
}


int main(int argc, char* argv[]) {
  char* filename;
  int read_status = -1;
  
  Graph* graph = (Graph*) malloc(sizeof(Graph));

  filename = (char*) malloc(128 * sizeof(char));
  
  if (argc > 1) {
    filename = argv[1];
    read_status = read_graph(filename, graph);
    if (read_status == -1) {
      printf("File doesn't exist.\n");
    }
  }

  if (argc > 2) {
    if (strcmp(argv[2], "-p") == 0) {
      PRINT_MATRIX = 1;
    }
  }

  if (PRINT_MATRIX == 1) print_graph(graph);

  Arcs* matching = (Arcs*) malloc(sizeof(Arcs));

  /*matching = hungarian(graph);

  int i,j;

  for (i = 0 ; i < matching->n ; i++) {
    printf("%d %d\n", matching->arcs[0][i], i);  
  }*/

  Bipartite_Graph* bg = (Bipartite_Graph*) malloc(sizeof(Bipartite_Graph));
  
  bipartite(graph, bg, 0);

  print_set(bg->X);
  printf("\n");
  print_set(bg->Y);

  return 0;
}
