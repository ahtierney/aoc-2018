#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "BSTNode.h"
#include "Grid.h"

#include "printhelpers.h"

BSTNode * newNode(int value) {
  BSTNode *entry = malloc(sizeof(BSTNode));
  entry->value = value;
  entry->left = NULL;
  entry->right = NULL;
  return entry;
}

void releaseTree(BSTNode *head) {
  if (head->right) {
    releaseTree(head->right);
  } else if (head->left) {
    releaseTree(head->left);
  }
  free(head);
}

void insertInto(BSTNode *head, int value) {
  if (head->value > value) {
    if (head->left) {
      insertInto(head->left, value);
    } else {
      head->left = newNode(value);
    }
  } else if (head->value < value) {
    if (head->right) {
      insertInto(head->right, value);
    } else {
      head->right = newNode(value);
    }
  }
}

void performRemoval(BSTNode *head, int value) {
  BSTNode *toRemove;
  if (head->right && head->right->value == value) { 
    toRemove = head->right;
  } else {
    toRemove = head->left;
  }

  if (toRemove->right && toRemove->left) {
    // we have two children, reformat the tree:
    // find the smallest element in the right subtree;
    BSTNode *min = toRemove->right;
    BSTNode *minParent = toRemove;
    // only take one left turn, once we've taken 1 we can rule out the right
    // side of the tree
    bool leftTurn = false;
    while (true) {
      if (min->left) {
        minParent = min;
        min = min->left;
        leftTurn = true;
      } else if (!leftTurn && min->right) {
        minParent = min;
        min = min->right;
      } else {
        break;
      }
    }
    // replace toRemove with that value
    toRemove->value = min->value;
    // remove the minimum value:
    performRemoval(minParent, min->value);  
    return;
  } else if (toRemove->right) {
    // one child: move the node up one level
    if (head->right == toRemove) head->right = toRemove->right;
    else if (head->left == toRemove) head->left = toRemove->right;
  } else if (toRemove->left) {
    // one child: move the node up one level
    if (head->right == toRemove) head->right = toRemove->left;
    else if (head->left == toRemove) head->left = toRemove->left;
  } else {
    // no children, just null the reference
    if (head->right == toRemove) head->right = NULL;
    else if (head->left == toRemove) head->left = NULL;
  }

  // clean up the dead node:
  toRemove->right = NULL;
  toRemove->left = NULL;
  releaseTree(toRemove);
}

void removeFrom(BSTNode *head, int value) {
  if ((head->right && head->right->value == value) ||
      (head->left && head->left->value == value)) {
    // found the node, perform removal
    performRemoval(head, value);
  } else {
    if (head->left) removeFrom(head->left, value);
    if (head->right) removeFrom(head->right, value);
  }
}

bool treeHas(BSTNode *head, int value) {
  if (head->value == value) {
    return true;
  } else if (head->value > value) {
    if (head->left) {
      return treeHas(head->left, value);
    }
    return false;
  } else if (head->value < value) {
    if (head->right) {
      return treeHas(head->right, value);
    }
    return false;
  }
  assert(false); // value must be > < or ==
}

void buildPrintTree(BSTNode *head, int **headIndices, Grid *grid, int depth) { 

  int *heads = headIndices[0];

  if ((depth + 2) >= grid->width) {
    int oldWidth = grid->width;
    expandWidth(grid, 100);
    // keep parity with grid width:
    heads = realloc(heads, sizeof(int) * grid->width);
    // set new memory to 0:
    int delta = (grid->width - oldWidth);
    memset(&heads[oldWidth - 1], 0, sizeof(int) * delta);
    // update the pointer to the heads array
    headIndices[0] = heads;
  }

  // printf("ptr %p\n", heads);
  printf("dep %d\n", depth);
  // printIntArray(heads, grid->width);

  if (heads[depth] >= grid->height) {
    expandHeight(grid, 30);
  }

  if (head->left) {
    int idx = heads[depth];
    grid->grid[depth][idx] = head->left->value;
    printf("l v %d (idx: %d)\n", head->left->value, idx);
    heads[depth] = idx + 1;
  }

  if (head->right) {
    int idx = heads[depth];
    grid->grid[depth][idx] = head->right->value;
    printf("r v %d (idx: %d)\n", head->right->value, idx);
    heads[depth] = idx + 1;
  }

  depth++;

  if (head->left) buildPrintTree(head->left, headIndices, grid, depth);
  if (head->right) buildPrintTree(head->right, headIndices, grid, depth);
}

void printTree(BSTNode *head) {
  Grid *g = newGrid();
  expandWidth(g, 99);
  expandHeight(g, 29);

  int *heads = calloc(sizeof(int), g->width);

  g->grid[0][0] = head->value;
  buildPrintTree(head, &heads, g, 1);

  // head node:
  printf("0 -> %d\n", g->grid[0][0]);

  for(int i = 1; i<g->width; i++) {
    // escape when we hit 0 for that depth, there must always be one node per depth
    if (heads[i] == 0) break;
    printf("%d -> ", i);
    for (int j = 0; j < heads[i]; j++) {
      printf("%d ", g->grid[i][j]);
    }
    printf("\n");
  }
  free(heads);
  freeGrid(g);
}
