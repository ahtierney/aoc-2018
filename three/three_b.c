#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "utils.h"

void checkPattern(BSTNode* lookUpHead, Grid *g, int id, int x, int y, int width, int height) {
  int upperX = x + width;
  if (g->width < upperX) {
    expandWidth(g, upperX - g->width);
  }

  int upperY = y + height;
  if (g->height < upperY) {
    expandHeight(g, upperY - g->height);
  }

  bool collide = false;

  for (int i=x; i<(x + width); i++) {
    for (int j=y; j<(y + height); j++) {
      if (g->grid[i][j] != 0) {
        collide = true;
        int conflictingId = g->grid[i][j];
        removeFrom(lookUpHead, conflictingId);
      } else {
        g->grid[i][j] = id;
      }
    }
  }

  if (!collide) {
    insertInto(lookUpHead, id);
  }
}

int main(int argc, char *argv[]) {
  assert(argc == 2);

  FILE *f;
  f = fopen(argv[1], "r");
  assert(f);

  BSTNode *lookUpTree = newNode(0);
  Grid *grid = newGrid();

  int id = 0, x = 0, y = 0, w = 0, h = 0;
  while(fscanf(f, "#%d @ %d,%d: %dx%d\n", &id, &x, &y, &w, &h) != EOF) {
    checkPattern(lookUpTree, grid, id, x, y, w, h);
  }
  freeGrid(grid);
  fclose(f);
  printTree(lookUpTree);

  releaseTree(lookUpTree);
  return 0;
}
