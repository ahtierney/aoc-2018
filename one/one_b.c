#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "utils.h"

const char commaString[] = ",";
int readValue(char *arg) {
  int lenLessComma = strcspn(arg, commaString);
  assert(lenLessComma > 1);

  char sign = arg[0];
  assert(sign == '+' || sign == '-');

  int value, valueLen;

  // size of num - sign
  valueLen = (lenLessComma - 1);

  // copy the substring from sign to comma into value str
  char *valueStr;
  valueStr = malloc(sizeof(char) * valueLen + 1);
  memcpy(valueStr, arg+1, valueLen + 1);

  // read the value, clean up
  value = atoi(valueStr);
  free(valueStr);

  // return and accomidate negative
  bool negative = sign == '-';
  return negative ? value * -1 : value;
}

int main(int argc, char *argv[]) {
  int freq = 0;
  assert(argc == 2);

  FILE *f;
  f = fopen(argv[1], "r");
  assert(f);

  struct BSTNode *head;
  head = newNode(0);

  // can hold a 32 bit int + sign
  char *temp = malloc(sizeof(char) * 11);
  while(true) {
    if (fscanf(f, "%s\n", temp) == EOF) {
      rewind(f);
      continue;
    }
    
    freq += readValue(temp);
    if (treeHas(head, freq)) {
      break;
    } else {
      insertInto(head, freq);
    }
  };

  releaseTree(head);
  free(temp);
  fclose(f);

  printf("%d \n", freq);
  return 0;
}
