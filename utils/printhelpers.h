#ifndef PRINTHELPERS_H
#define PRINTHELPERS_H

void printIntArray(int *array, int len);
void printPointerArray(void **array, int len, void (printFn)(void *));
void printIntPtr(int *i);
void printStr(char *str);

#endif
