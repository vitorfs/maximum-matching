#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "set.h"

int PRINT_MATRIX = 0;

void print_arcs(Arcs* a) {
  printf("----------\n");
  int i;
  for (i = 0 ; i < a->n ; i++) {
    printf("%d %d\n", i, a->arcs[0][i]);  
  }
  printf("----------\n");
}

// Recebe um grafo g
// Retorna um emparelhamento maximal
// Deve ser reexecutado para todo vértice não M-saturado
Arcs* hungarian(Graph *g) {
   int i, j, u;
   HeaderSet *S, *T, *NS;
   Arcs *M;

   Bipartite_Graph* bg = (Bipartite_Graph*) malloc(sizeof(Bipartite_Graph));
   bipartite(g, bg, 0);

   init_header_set(S);
   init_header_set(T);
   init_header_set(NS);

   M = init_arcs(g->vertex_count);

   print_arcs(M); //**********************

   int break_loop = 0;
   for (i = 0 ; i < g->vertex_count ; i++) {
      for (j = i ; j < g->vertex_count ; j++)
         if (g->arcs[i][j] > 0) {
            insert_arcs(i, j, g->arcs[i][j], M);
            break_loop = 1; 
            break;
         }
      if (break_loop) 
        break;
   }

   print_arcs(M); //**************************
   u = non_saturation_header_set(bg->X, M);

   printf("\nX: "); //**************************
   print_set(bg->X); //**************************

   printf("\n%d\n", u); //**************************

   while (u >= 0) {
      int y;
      insert_header_set(u, S);
      printf("\nS: "); //**************************
      print_set(S->first); //**************************
      zero_header_set(T);
      zero_header_set(NS);
      printf("\nNS: "); //**************************
      print_set(NS->first); //**************************
      builds_neighborhood_header_set(S, NS, g);

      while (!compare_header_set(NS, T, g)) {
         y = saturation_header_set(subtraction_set(NS, T, g), M);
         if (y < 0) break;

         insert_header_set(M->arcs[0][y], S);
         insert_header_set(y, T);

         builds_neighborhood_header_set(S, NS, g);
      }
      if (compare_header_set(NS, T, g))
         return M;
         //return S;
      else {
         Arcs* P;
         P = init_arcs(g->vertex_count);
         y = non_saturation_header_set(subtraction_set(NS, T, g), M);

         P = augmenting_path(u, y, M, g);
         symmetric_difference_arcs(y, P, M);
      }

      u = non_saturation_header_set(bg->X, M);
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
  matching = hungarian(graph);
  //print_arcs(matching);
  return 0;
}
