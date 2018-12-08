#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <utils.h>

int alphaIndex(char character) {
  return (int) character - (int) 'a';
}

typedef struct CharPathNode CharPathNode;
struct CharPathNode {
  char character;
  int nextCharactersLen;
  int nextCharactersHead;
  CharPathNode **nextCharacters;
};

CharPathNode * newCharPathNode(char c) {
  CharPathNode *node = malloc(sizeof(CharPathNode));
  node->character = c;
  node->nextCharactersLen = 0;
  node->nextCharactersHead = 0; // len 26
  node->nextCharacters = NULL;
  return node;
}

CharPathNode * nodeMatchingCharacter(CharPathNode *node, char c) {
  CharPathNode *charNode;
  for (int i = 0; i < node->nextCharactersHead; i++) {
    charNode = node->nextCharacters[i];
    if (charNode->character == c) {
      return charNode;
    }
  }
  return NULL;
}

#define DEFAULT_BUCKET_INCREMENT 7
CharPathNode * addNextCharacter(CharPathNode *node, char nextCharacter) {
  // allocate array if null
  if (!node->nextCharacters) {
    node->nextCharactersLen = sizeof(CharPathNode) * DEFAULT_BUCKET_INCREMENT;
    node->nextCharacters = calloc(sizeof(CharPathNode), DEFAULT_BUCKET_INCREMENT);
  }
  
  // if we have the character in this position already, return early
  CharPathNode *nextCharNode = nodeMatchingCharacter(node, nextCharacter);
  if (nextCharNode) {
    printf("ext: %c -> %c\n", node->character, nextCharNode->character);
    return nextCharNode;
  } 

  nextCharNode = newCharPathNode(nextCharacter);

  // else add the new node:
  // add space if needed
  if (node->nextCharactersHead * sizeof(CharPathNode) == node->nextCharactersLen) {
    size_t newLen = node->nextCharactersLen + (DEFAULT_BUCKET_INCREMENT * sizeof(CharPathNode));
    node->nextCharacters = realloc(node->nextCharacters, newLen);
    node->nextCharactersLen = newLen;
  }

  node->nextCharacters[node->nextCharactersHead] = nextCharNode;
  node->nextCharactersHead++;
  printf("new: %c -> %c\n", node->character, nextCharNode->character);
  return nextCharNode;
}

CharPathNode * childNodeWithCharacter(CharPathNode *node, char c) {
  CharPathNode *charNode;
  printf("looking for children: ");
  for (int i = 0; i < node->nextCharactersHead; i++) {
    charNode = node->nextCharacters[i];
    printf("%c", charNode->character);
    CharPathNode *childNode = nodeMatchingCharacter(charNode, c);
    if (childNode) {
       printf("  match %c with %c \n", c, childNode->character);
      return charNode;
    }
  }
  printf("\n");
  return NULL;
}

int hasMatchingPath(CharPathNode *node, char *str) {
  size_t len = strlen(str);
  CharPathNode *head = node;
  int skippedIndex = -1;
  if (head->character != str[0]) {
    return -1;
  }
  // ignore first character, it should match node;
  for (int i=1; i<len; i++) {
    char c = str[i];
    CharPathNode *nextNode = nodeMatchingCharacter(head, c);
    if (nextNode) {
      printf("has next: %c\n", nextNode->character);
      head = nextNode;
    } else {
      printf("no next\n");
      if (skippedIndex != -1) {
        return -1;
      } else {
        if (i == len - 1) {
          return i;
        } else {
          skippedIndex = i;
          printf("trying with skip: %d\n", skippedIndex);
          // get the next char and see any of its children has that character
          char skipChar = str[i+1];
          printf("curnode: %c\n", head->character);
          CharPathNode *wildCardNode = childNodeWithCharacter(head, skipChar);
          if (wildCardNode) {
            printf("continuing with wild card: %c\n", wildCardNode->character);
            head = wildCardNode;
          } else {
            return -1;
          }
        }
      }
    }
  }
  return skippedIndex;
}

int main(int argc, char *argv[]) {
  assert(argc == 2);

  FILE *f;
  f = fopen(argv[1], "r");
  assert(f);

  // array representing a node for each letter in the alphabet
  CharPathNode **firstLetters = calloc(26, sizeof(CharPathNode));

  // ids are 26 characters + null
  char *temp = malloc(sizeof(char) * 27);
  // match string same size less one character
  char *matchingString = malloc(27 * sizeof(char));
  
  // pointer to current node
  CharPathNode *head = NULL;
  while(fscanf(f, "%s\n", temp) != EOF) {
    printf("%s\n", temp);
    // set the start to a specific letter position
    char firstLetter = temp[0];
    int idx = alphaIndex((int) firstLetter);
    if (!firstLetters[idx]) {
      firstLetters[idx] = newCharPathNode(firstLetter);
    }
    // set head
    head = firstLetters[idx];

    int skipIdx = hasMatchingPath(head, temp);
    printf("%d \n", skipIdx);
    if (skipIdx != -1) {
      strcpy(matchingString, temp);
      matchingString[skipIdx] = '!';
      break;
    }

    // iterate through the remaining 25 chars
    size_t len = strlen(temp);
    for(int i = 1; i<len; i++) {
      char c = temp[i];
      CharPathNode *next = addNextCharacter(head, c);
      head = next;
    }
  }
  
  free(temp);
  fclose(f);
  printf("%s\n", matchingString);
  return 0;
}
