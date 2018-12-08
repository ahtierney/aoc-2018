#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <utils.h>

int countAndCoverSquares(Grid *g, int x, int y, int width, int height) {
  int upperX = x + width;
  if (g->width < upperX) {
    expandWidth(g, upperX - g->width);
  }

  int upperY = y + height;
  if (g->height < upperY) {
    expandHeight(g, upperY - g->height);
  }

  int ret = 0;
  for (int i=x; i<(x + width); i++) {
    for (int j=y; j<(y + height); j++) {
      if (g->grid[i][j] == (short) 1) ret++;
      if (g->grid[i][j] < 2) g->grid[i][j]++;
    }
  }
  return ret;
}

int main(int argc, char *argv[]) {
  assert(argc == 2);

  FILE *f;
  f = fopen(argv[1], "r");
  assert(f);

  Grid *grid = newGrid();

  int result = 0;
  int id = 0, x = 0, y = 0, w = 0, h = 0;
  while(fscanf(f, "#%d @ %d,%d: %dx%d\n", &id, &x, &y, &w, &h) != EOF) {
    result += countAndCoverSquares(grid, x, y, w, h);
  }
  freeGrid(grid);
  fclose(f);
  printf("%d\n", result);
  return 0;
}
