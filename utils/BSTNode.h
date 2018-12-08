#ifndef BSTNODE_H
#define BSTNODE_H

typedef struct BSTNode BSTNode;
struct BSTNode {
  int value;
  struct BSTNode *left;
  struct BSTNode *right;
};

BSTNode * newNode(int value);
void insertInto(BSTNode *head, int value);
void removeFrom(BSTNode *head, int value);
bool treeHas(BSTNode *head, int value);
void printTree(BSTNode *head);
void releaseTree(BSTNode *head);

#endif
