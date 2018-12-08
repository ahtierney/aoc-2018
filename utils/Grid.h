
typedef struct Grid Grid;
struct Grid {
  int width;
  int height;
  int **grid;
};

Grid * newGrid();
void freeGrid(Grid *g);
void expandWidth(Grid *g, int distance);
void expandHeight(Grid *g, int distance);
