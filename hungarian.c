#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "set.h"

// Recebe um grafo g
// Retorna um emparelhamento maximal
// Deve ser reexecutado para todo vértice não M-saturado
Arcs* hungarian(Graph *g) {
   int i, j, u;
   HeaderSet *S = (HeaderSet*) malloc(sizeof(HeaderSet));
   HeaderSet *T = (HeaderSet*) malloc(sizeof(HeaderSet));
   HeaderSet *NS = (HeaderSet*) malloc(sizeof(HeaderSet));
   HeaderSet *X = (HeaderSet*) malloc(sizeof(HeaderSet));
   HeaderSet *Y = (HeaderSet*) malloc(sizeof(HeaderSet));
   Arcs *M;

   init_header_set(X);
   init_header_set(Y);
   // Bipartite_Graph* bg = (Bipartite_Graph*) malloc(sizeof(Bipartite_Graph));
   bipartite(g, X, Y);
   //print_set(X->first);
   //puts("");
   //print_set(Y->first);
   //puts("");
   //getchar();

   init_header_set(S);
   init_header_set(T);
   init_header_set(NS);

   M = init_arcs(g->vertex_count);

   //print_arcs(M);
   //getchar();

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

   //print_arcs(M);
   //getchar();

   u = non_saturation_header_set(X, M);
   //printf("%d\n", u);

   while (u >= 0) {
      int y;

      insert_header_set(u, S);
      //print_set(S->first);
      //puts("");

      zero_header_set(T);
      //print_set(T->first);
      //puts("");

      zero_header_set(NS);
      //print_set(NS->first);
      builds_neighborhood_header_set(S, NS, g);
      //print_set(NS->first);
      //puts("");

      while (!compare_header_set(NS, T, g)) {
//*
         puts("Step 1");
         printf("NS nodes: %d\nT nodes: %d\n", NS->nodes, T->nodes);
         print_set(NS->first);
         puts("- NS");
         print_set(S->first);
         puts(" - S");
         print_set(T->first);
         puts("- T");
         puts("");
         getchar();

         puts("Step 2");
         HeaderSet *asdf = subtraction_header_set(NS, T, g);
         print_set(asdf->first);
         puts("- NS\\T");
         print_arcs(M);

         y = saturation_header_set(subtraction_header_set(NS, T, g), M);
         printf("Saturation: %d\n", y);
         if (y < 0) break;

         insert_header_set(M->arcs[0][y], S);
         print_set(S->first);
         puts(" - S");
         insert_header_set(y, T);
         print_set(T->first);
         puts(" - T");

         builds_neighborhood_header_set(S, NS, g);
         print_set(NS->first);
         puts(" - NS");
         getchar();
      }
      if (compare_header_set(NS, T, g))
         break;
         //return S;
      else {
         Arcs* P;
         P = init_arcs(g->vertex_count);
         y = non_saturation_header_set(subtraction_header_set(NS, T, g), M);
         puts("Step 3");
         printf("Non-saturation: %d\n", y);

         P = augmenting_path(u, y, M, g);
         print_arcs(P);
         // Se não existe um caminho aumentante de y para u retorna M
         if (P == NULL)
            break;
         symmetric_difference_arcs(y, P, M);
         print_arcs(M);
         getchar();
      }

      u = non_saturation_header_set(X, M);
      printf("%d\n", u);
   }

   return M;
}
