#include "graph.h"
#include "set.h"
//#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

Set* init_set() {
   return NULL;
}

void init_header_set(HeaderSet *head) {
   head->nodes = 0;
   head->first = init_set();
}

void empty_arcs(Arcs *e) {
   e->arcs = NULL;
}

Arcs* init_arcs(int n) {
   int i;
   Arcs *e = (Arcs*) malloc(sizeof(Arcs));
   e->n = n;

   empty_arcs(e);

   e->arcs = (int**) malloc(2 * sizeof(int*));
   e->arcs[0] = (int*) malloc(n * sizeof(int));
   e->arcs[1] = (int*) malloc(n * sizeof(int));

   for (i = 0 ; i < n ; i++)
      e->arcs[0][i] = -1;
   for (i = 0 ; i < n ; i++)
      e->arcs[1][i] = -1;

   return e;
}

void free_set(Set *a) {
   Set *temp;
 
   while (a != NULL) {
      temp = a;
      a = a->next;
      free(temp);
   }
}

void free_header_set(HeaderSet *head) {
   free_set(head->first);
   head->first = init_set();

   free(head);
}

void free_arcs(Arcs *e) {
   free(e->arcs[0]);
   free(e->arcs[1]);
   free(e->arcs);
   empty_arcs(e);

   free(e);
}

void zero_header_set(HeaderSet *head) {
   free_set(head->first);
   init_header_set(head);
}

void zero_arcs(Arcs *e) {
   int i;

   for (i = 0 ; i < e->n ; i++)
      e->arcs[0][i] = -1;
   for (i = 0 ; i < e->n ; i++)
      e->arcs[1][i] = -1;
}

void insert_header_set(int v, HeaderSet *head) {
   Set *temp = head->first;
   Set *new;

   while (temp != NULL) {
      if (temp->vertex == v)
         return;
      temp = temp->next;
   }

   new = (Set*) malloc(sizeof(Set));
   new->vertex = v;
   new->next = head->first;
   head->first = new;
   head->nodes++;
}

void insert_arcs(int u, int v, int weight, Arcs *M) {
   M->arcs[0][u] = v;
   M->arcs[0][v] = u;
   M->arcs[1][u] = weight;
   M->arcs[1][v] = weight;
}

void remove_header_set(int v, HeaderSet *head) {
   Set *a = head->first;
   Set *temp = init_set();

   // Caso onde o vértice a ser removido é o primeiro elemento
   if (a->vertex == v) {
      temp = a;
      a = a->next;
      free(temp);
      // Sendo o primeiro elemento deve-se alterar o valor de head->first
      head->first = a;
      head->nodes--;
   } else {
      while(a != NULL) {
         if (a->next->vertex == v) {
            temp = a->next;
            a->next = a->next->next;
            free(temp);
            head->nodes--;
            break; // Caso encontre o vértice, pode interromper a busca (desempenho)
         }
         a = a->next;
      }
   }
}

void remove_arcs(int u, int v, Arcs *M) {
   M->arcs[0][u] = -1;
   M->arcs[0][v] = -1;
   M->arcs[1][u] = -1;
   M->arcs[1][v] = -1;
}

int exist_vertex_header_set(int v, HeaderSet *head) {
   Set *a = head->first;

   for ( ; a != NULL; a = a->next)
      if (a->vertex == v)
         return 1;

   return 0;
}

int non_saturation_header_set(HeaderSet *head, Arcs *M) {
   Set *a = head->first;
   int v;
   
   while (a != NULL) {
      v = a->vertex;
      if (M->arcs[0][v] < 0)
         return v;
      a = a->next;
   }
   return -1;
}

int saturation_header_set(HeaderSet *head, Arcs *M) {
   Set *a = head->first;
   int v;

   while (a != NULL) {
      v = a->vertex;
      if (M->arcs[0][v] >= 0)
         return v;
      a = a->next;
   }
   return -1;
}

void builds_neighborhood_header_set(HeaderSet *S, HeaderSet *NS, Graph *g) {
   Set *s_list = S->first;
   int v, j;
   
   while (s_list != NULL) {
      v = s_list->vertex;
      for (j = 0 ; j < g->vertex_count ; j++) {
         if (g->arcs[v][j] > 0)
            insert_header_set(j, NS);
      }
      s_list = s_list->next;
   }
}

int compare_header_set(HeaderSet *NS, HeaderSet *T, Graph *g) {
   // Se o número de elementos for diferente o conjunto não é igual (desempenho)
   if (NS->nodes != T->nodes)
      return 0;

   // Vetor auxiliar para evitar que comparação de duas lista leve O(nm)
   int *t = (int*) calloc(g->vertex_count, sizeof(int));
   Set *ns_list = NS->first;
   Set *t_list = T->first;

   while (t_list != NULL) {
      t[t_list->vertex] = 1;
      t_list = t_list->next;
   }

   while (ns_list != NULL) {
      if(!t[ns_list->vertex])
         return 0;
      ns_list = ns_list->next;
   }

   return 1;
}

HeaderSet* subtraction_header_set(HeaderSet *NS, HeaderSet *T, Graph *g) {
   // Vetor auxiliar para evitar que comparação de duas lista leve O(nm)
   int *t = (int*) calloc(g->vertex_count, sizeof(int));
   Set *ns_list = NS->first;
   Set *t_list = T->first;
   HeaderSet *Subtraction = (HeaderSet*) malloc(sizeof(HeaderSet));
   init_header_set(Subtraction);

   while (t_list != NULL) {
      t[t_list->vertex] = 1;
      t_list = t_list->next;
   }

   while (ns_list != NULL) {
      if(!t[ns_list->vertex])
         insert_header_set(ns_list->vertex, Subtraction);
      ns_list = ns_list->next;
   }

   return Subtraction;
}

void symmetric_difference_arcs(int y, Arcs *P, Arcs *M) {
   while (P->arcs[0][y] >= 0) {
      if (M->arcs[0][y] != P->arcs[0][y])
         insert_arcs(y, P->arcs[0][y], 0, M);
      else
         remove_arcs(y, P->arcs[0][y], M);
      y = P->arcs[0][y];
   }
}

Arcs* augmenting_path(int u, int y, Arcs *M, Graph *g) {
   int i;
   int z;
   Arcs *P = init_arcs(g->vertex_count);

   P->arcs[1][y] = 1;
   for ( ; ; ) {
      // Procura vértices incidentes em y
      for (z = -1 ; i < g->vertex_count ; i++)
         if (g->arcs[y][i] > 0 && P->arcs[1][i] != 1)
            z = i;

      // Caso não encontre vértices incidentes em y, PARE.
      // Ou seja, z permanece com o valor definido acima
      if (z == -1)
         break;

      // Se o vértice encontrado é o próprio vértice livre u
      // retorna a aresta, pois é um caminho aumentante
      if (z == u) {
         P->arcs[0][y] = z;
         return P;
      // se não, se vértice z é M-saturado, faça:
      } else if (M->arcs[0][z] >= 0) {
         P->arcs[0][y] = z;   // z é o próximo salto de y
         P->arcs[1][z] = 1;   // marca z como visitado
         y = M->arcs[0][z];   // y recebe o vértice vizinho de z em M
         P->arcs[0][z] = y;   // y é o próximo salto de z
         P->arcs[1][y] = 1;   // marca y como visitado

         i = 0;   // zera i para recomeçar a busca a partir do novo y
      }
      // caso contrário procure outro vértice a partir de i
   }

   return NULL;
}

void print_set(Set* s) {
   if (s == NULL)
      printf("O conjunto estah nulo.");
   else {
      while (s != NULL) {
         printf("%d, ", s->vertex);
         s = s->next;
      }
   }
}

void bipartite_define_header_set(int v, int *aux, Graph *g, HeaderSet *X, HeaderSet *Y) {

   int i, j;

   insert_header_set(v, X);
   aux[v] = 1;

   for (j = 0 ; j < g->vertex_count ; j++)
      if (g->arcs[v][j] > 0 && aux[j] == 0)
         bipartite_define_header_set(j, aux, g, Y, X);
}

void bipartite(Graph *g, HeaderSet *X, HeaderSet *Y) {
   // Estrutura para auxiliar a busca por profundidade
   // Evita que vértices sejam inseridos mais de uma vez
   // Otimiza o processo se comparada com o uso da função exist_vertex_set()
   int aux[g->vertex_count];
   int i;

   for (i = 0 ; i < g->vertex_count ; i++)
      aux[i] = 0;

   for (i = 0 ; i < g->vertex_count ; i++)
      if (aux[i] == 0)
         bipartite_define_header_set(i, aux, g, X, Y);
}

/*
void bipartite(Graph* g, Bipartite_Graph* bg, int init_vertex) {
  Stack* open = (Stack*) malloc(sizeof(Stack));
  int* visited = (int*) calloc(g->vertex_count, sizeof(int));
  int* partition = (int*) malloc(g->vertex_count * sizeof(int));

  int i;
  for (i = 0 ; i < g->vertex_count ; i++) {
    partition[i] = -1;
  }

  push(open, init_vertex);
  partition[init_vertex] = 0;

  while (!empty_stack(open)) {
    int v = pop(open);
    for (i = 0 ; i < g->vertex_count ; i++) {
      if (g->arcs[v][i] > 0 && visited[i] == 0) {
        push(open, i);
        partition[i] = (partition[v] + 1) % 2;
      }
    }
    visited[v] = 1;
  }

  bg->X = init_set();
  bg->Y = init_set();

  for (i = 0 ; i < g->vertex_count ; i++) {
    if (partition[i] == 0)
      bg->X = insert_set(i, bg->X);
    if (partition[i] == 1)
      bg->Y = insert_set(i, bg->Y);
  }
}
*/
