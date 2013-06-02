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

int PRINT_MATRIX = 0;

/*
 * The read_graph function takes a pointer to char as parameter which represents
 * the name of the file which contains the information of the graph we want 
 * to work. After reading the information, this function converts the text to 
 * a valid graph representation.
 * This function will return 0 in case of success or -1 if any error occur. 
 */
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

/*
 * This function has the job to print the matching result set from a graph g
 */
void print_matching(Graph* g) {
  int i, j;
  int arc_count = 0;

  if (PRINT_MATRIX == 1) print_graph(g);

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
  //Vertex* v = NULL;
  get_ordered_vertex(m, v);
  
  int saturated[m->vertex_count];
  for (i = 0 ; i < m->vertex_count ; i++)
    saturated[i] = 0;

  int* adj;
  for (i = 0 ; i < m->vertex_count ; i++) {
    int v1 = v[i].vertex;
    int v2;

    if (saturated[v1] == 0) {
      adj = get_adjacency(m, v1);
      if (adj[0] > 0) {
        v2 = adj[1];
        
        for (j = 2 ; j <= adj[0] ; j++) {
          //if (get_vertex_degree(m, adj[j]) < get_vertex_degree(m, v2)) v2 = adj[j];
          if (m->vertex[adj[j]].degree < m->vertex[v2].degree) v2 = adj[j];
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

Graph* hungarian(Graph* g) {
  Graph* m = (Graph*) malloc(sizeof(Graph));

  init_graph(m, g->vertex_count);
  
  int i;
  int j;

  int* s = (int*) malloc(sizeof(int));
  s[0] = 0;

}

/*
 * The main program optionally accepts one argv that describes the filename
 * for converting the graph to the working data structure
 */
int main(int argc, char* argv[]) {
  char* filename;
  int read_status = -1;
  
  Graph* graph = (Graph*) malloc(sizeof(Graph));
  Graph* gmatching = (Graph*) malloc(sizeof(Graph));

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

  if (read_status == -1) {
    printf("Type the filename or 'exit' to quit the program.\n");
    do {
      scanf("%128s", filename);
      if (strcmp(filename, "exit") == 0) {
        return 0;
      }
      read_status = read_graph(filename, graph);
      if (read_status == -1) {
        printf("File doesns't exist.\n");
      }
    } while (read_status == -1);

  }

  if (PRINT_MATRIX == 1) print_graph(graph);

  gmatching = maximal_matching(graph);

  print_matching(gmatching);

  return 0;
}
