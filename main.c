#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "maximum-matching.h"
#include "hungarian.h"
#include "set.h"

int PRINT_MATRIX = 0;

int main(int argc, char* argv[]) {
  char* filename;
  int read_status = -1;
  
  Graph* graph = (Graph*) malloc(sizeof(Graph));
  Graph* gmatching = (Graph*) malloc(sizeof(Graph));
  Arcs* matching = (Arcs*) malloc(sizeof(Arcs));
  
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

  int option;
  printf("Type 1 for general graph or 2 for bipartite graph or 0 to exit.\n");
  scanf("%d", &option);

  if (PRINT_MATRIX == 1) print_graph(graph);

  switch(option) {
    case 1:
      gmatching = maximal_matching(graph);
      print_matching(gmatching);
      break;
    case 2:
      matching = hungarian(graph);
      print_arcs(matching);
      break;
  }

  return 0;
}
