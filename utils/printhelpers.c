#include <stdio.h>

void printIntArray(int *array, int len) {
  for (int i = 0; i<len; i++) {
    printf("%d,", array[i]);
  }
  printf("\n");
}

void printPointerArray(void **array, int len, void (printFn)(void *)) {
  for (int i = 0; i<len; i++) {
    if (array[i]) {
      printFn(array[i]);
      printf(", ");
    } else {
      printf("0, ");
    }
  }
  printf("\n");
}

void printIntPtr(int *i) {
  printf("%d", *i);
}

void printStr(char *str) {
  printf("%s", str);
}
