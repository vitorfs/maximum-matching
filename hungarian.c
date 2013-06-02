#include "graph.h"
#include "set.h"

// Recebe um grafo g
// Retorna um emparelhamento maximal
// Deve ser reexecutado para todo vértice não M-saturado
Arcs* hungarian(Graph *g) {
   int i, j, u;
   Set *S, *T, *NS, *X, *Y;
   Arcs *M;

   X = init_set();
   Y = init_set();
   bipartite_define_set(0, g, X, Y)

   S = init_set();
   T = init_set();
   NS = init_set();

   init_arcs(M, g->vertex_count);
   for (i = 0 ; i < g->vertex_count ; i++)
      for (j = i ; j < g->vertex_count ; j++)
         if (g->arcs[i][j] > 0)
            insert_arcs(i, j, M);

   u = non_saturation_set(X, M);
   while (u >= 0) {

      int y;

      insert_set(u, S);
      T = init_set();
      builds_neighborhood_set(S, NS);

      while (!compare_set(NS, T)) {
         y = saturation_set(subtraction_set(NS, T), M);
         if (y < 0) break;

         insert_set(M->arcs[0][y], S);
         insert_set(y, T);

         builds_neighborhood_set(S, NS);
      }
      if (compare_set(NS, T))
         return M;
         //return S;
      else {
         Arcs* P = NULL;
         y = non_saturation_set(subtraction_set(NS, T), M);

         P = augmenting_path(y, g);
         M = symmetric_difference_arcs(M, P);
      }

      u = non_saturation_set(X, M);
   }

   return M;
}
