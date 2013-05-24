#include "graph.h"

void swap(Vertex* x, Vertex* y) {
  Vertex z = *x;
  *x = *y;
  *y = z;
}

int partition(Vertex* array, int p, int r) {
  int pivot, i, j;

  pivot = array[r].degree;
  i = p - 1;

  for (j = p ; j < r ; j++) {
    if (array[j].degree <= pivot) {
      i++;
      swap(&array[i], &array[j]);
    }
  }

  swap(&array[i + 1], &array[r]);

  return i + 1;
}

void quicksort(Vertex* array, int p, int r) {
  int q;
  if (p <= r) {
    q = partition(array, p, r);
    quicksort(array, p, q - 1);
    quicksort(array, q + 1, r);
  }
}
