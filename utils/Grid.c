#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <Grid.h>

Grid * newGrid() {
  Grid *g = malloc(sizeof(Grid));
  g->width = 1;
  g->height = 1;
  int *y = calloc(sizeof(short), 1);
  int **x = calloc(sizeof(short*), 1);
  x[0] = y;
  g->grid = x;
  return g;
}

void freeGrid(Grid *g) {
  for (int i=0; i<g->width; i++) {
    free(g->grid[i]);
  }
  free(g->grid);
  free(g);
}

void expandWidth(Grid *g, int distance) {
  int oldWidth = g->width;
  int height = g->height;
  
  g->width += distance;
  g->grid = realloc(g->grid, sizeof(short*) * g->width);
  for(int i = oldWidth; i<g->width; i++) {
    g->grid[i] = calloc(sizeof(short*), height);
  }
}

void expandHeight(Grid *g, int distance) {
  int width = g->width;
  g->height += distance;
  int height = g->height;
  for (int i = 0; i<width; i++) {
    g->grid[i] = realloc(g->grid[i], sizeof(short*) * height);
  }
}
