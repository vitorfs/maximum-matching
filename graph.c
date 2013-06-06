#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "quicksort.h"

void empty_graph(Graph* g) {
  g->vertex_count = 0;
  g->arcs = NULL;
  //g->vertex = NULL;
}

void init_graph(Graph* g, int n) {
  int i, j;
  empty_graph(g);

  g->vertex_count = n;
  g->arcs = (int**) calloc(n, sizeof(int*));
  for (i = 0 ; i < g->vertex_count ; i++)
    g->arcs[i] = (int*) calloc(n, sizeof(int));
  g->vertex = (Vertex*) malloc(n * sizeof(Vertex));
  for (i = 0 ; i < g->vertex_count ; i++) {
    g->vertex[i].vertex = i;
    g->vertex[i].degree = 0;
    g->vertex[i].saturated = 0;
  }
}

void free_graph(Graph* g) {
  int i;

  for (i = 0 ; i < g->vertex_count ; i++)
    free(g->arcs[i]);
  //free(g->vertex);
  free(g);
  g = NULL;
}

int insert_arc(Graph* g, int a1, int a2, int weight) {
  if (a1 >= 0 && a1 < g->vertex_count && a2 >= 0 && a2 < g->vertex_count && g->arcs[a1][a2] == 0) {
    g->arcs[a1][a2] = weight;
    g->arcs[a2][a1] = weight;
    //g->vertex[a1].degree++;
    //g->vertex[a2].degree++;
    //printf("a1 %d %d\n\n", a1, g->vertex[a1].degree);
    return 0;
  }
  return -1;
}

int remove_arc(Graph* g, int a1, int a2) {
  int weight = -1;
  if (a1 >= 0 && a1 < g->vertex_count && a2 >= 0 && a2 < g->vertex_count && g->arcs[a1][a2] > 0) {
    weight = g->arcs[a1][a2];
    g->arcs[a1][a2] = 0;
    g->arcs[a2][a1] = 0;
    //g->vertex[a1].degree--;
    //g->vertex[a2].degree--;
  }
  return weight;
}

int exists_arc(Graph* g, int a1, int a2) {
  return g->arcs[a1][a2] > 0;
}

int* get_adjacency(Graph* g, int v) {
  int degree;

  degree = get_vertex_degree(g, v);

  int* adjacency = (int*) malloc((degree + 1) * sizeof(int));

  adjacency[0] = degree; // Using the first position of the pointer to determine the size of the array
  
  int i;
  int j;
  for (i = 0, j = 1 ; j <= degree ; i++) {
    if (g->arcs[i][v] > 0) {
      adjacency[j++] = i;
    }
  }

  return adjacency;
}

int get_vertex_degree(Graph* g, int v) {
  int i;
  int degree = 0;

  for (i = 0 ; i < g->vertex_count ; i++)
    if (g->arcs[i][v] > 0)
      degree++;

  return degree;
}

void get_ordered_vertex(Graph* g, Vertex* v) {
  //v = (Vertex*) malloc(g->vertex_count * sizeof(Vertex));
  
  int i;

  for (i = 0 ; i < g->vertex_count ; i++) {
    v[i].vertex = i;
    v[i].degree = get_vertex_degree(g, i);
    //v[i].degree = g->vertex[i].degree;
  }
  
  quicksort(v, 0, g->vertex_count - 1);
  //insertion_sort(v, g->vertex_count);
  /*for (i = 0 ; i < g->vertex_count ; i++) {
    printf("[%d (%d)]", v[i].vertex, v[i].degree);
  }
  printf("\n");
  for (i = 0 ; i < g->vertex_count ; i++) {
    printf("[%d (%d)]", g->vertex[i].vertex, g->vertex[i].degree);
  }*/
}

void insert_vertex(Graph* g, int v) {
  if (g->arcs == NULL) {
    init_graph(g, v); 
  }

  else {
    int i, j;
    g->vertex_count += v;
    g->arcs = (int**) realloc(g->arcs, g->vertex_count * sizeof(int*));
    
    for (i = 0 ; i < g->vertex_count - v ; i++)
      g->arcs[i] = (int*) realloc(g->arcs[i], g->vertex_count * sizeof(int)); // realloc the part of the matrix which were used before

    for ( ; i < g->vertex_count ; i++) 
      g->arcs[i] = (int*) calloc(g->vertex_count, sizeof(int)); // alloc the new part of the matrix

    for (i = 0 ; i < g->vertex_count - v ; i++) {
      for (j = g->vertex_count - v ; j < g->vertex_count ; j++) {
        g->arcs[i][j] = 0;
      }
    }
  }
}

int remove_vertex(Graph* g, int v) {
  int i, j;

  if (v == g->vertex_count - 1) {
    for (i = 0 ; i < g->vertex_count ; i++)
      g->arcs[i][g->vertex_count - 1] = g->arcs[i][v];

    for (j = 0 ; j < g->vertex_count ; i++)
      g->arcs[g->vertex_count - 1][j] = g->arcs[v][j];
  }

  g->vertex_count--;

  free(g->arcs[g->vertex_count - 1]);
  g->arcs = (int**) realloc(g->arcs, g->vertex_count * sizeof(int*));

  for (i = 0 ; i < g->vertex_count ; i++)
    g->arcs[i] = (int*) realloc(g->arcs[i], g->vertex_count * sizeof(int));

}

void print_graph(Graph* g) {
  int i, j;

  printf("  ");
  for (i = 0 ; i < g->vertex_count ; i++) printf(" %d ", i);
  for (i = 0 ; i < g->vertex_count ; i++) {
	 printf("\n");
    printf("%d ", i);
    for (j = 0 ; j < g->vertex_count ; j++) {
      printf("[%d]", g->arcs[i][j]);
    }
  }
  printf("\n\n");
}

void print_adjacency(int* a) {
  int i;

  for (i = 1 ; i <= a[0] ; i++)
    printf("[%d]", a[i]);

  if (a[0] == 0) printf("\n");

  printf("\n");
}

int read_graph(char* filename, Graph* g) {
  FILE *PTRFILE;

  PTRFILE = fopen(filename, "rt");
  
  if (PTRFILE == NULL) {
    return -1;
  }
  
  else {
    empty_graph(g);
  
    char line[80];
    char str_aux[80];
    int i;
    int j;
    int k;
    int arc_1;
    int arc_2;
    int weight = 0;
    
    for (i = 0 ; fgets(line, 80, PTRFILE) != NULL ; i++) {
      if (i == 0) {
        insert_vertex(g, atoi(line));
      }

      else {
        for (j = 0, k = 0 ; j < 80 && line[j] != 32 && line[j] != '\0' ; j++, k++) 
          str_aux[k] = line[j];

        str_aux[k] = '\0';
        arc_1 = atoi(str_aux);
        
        j = j + 1;
        str_aux[0] = '\0';

        for (k = 0 ; j < 80 && line[j] != 32 && line[j] != '\0' ; j++, k++)
          str_aux[k] = line[j];

        str_aux[k] = '\0';
        arc_2 = atoi(str_aux);
        
        j = j + 1;
        str_aux[0] = '\0';

        for (k = 0 ; j < 80 && line[j] != 32 && line[j] != '\0' ; j++, k++)
          str_aux[k] = line[j];

        str_aux[k] = '\0';
        weight = atoi(str_aux);

        if (weight == 0) weight = 1;

        insert_arc(g, arc_1, arc_2, weight);
      }

    }

  }
  return 0;
}
