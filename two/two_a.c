#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

int alphaIndex(char character) {
  return (int) character - (int) 'a';
}

int main(int argc, char *argv[]) {
  int twos = 0, threes = 0;
  assert(argc == 2);

  FILE *f;
  f = fopen(argv[1], "r");
  assert(f);

  // each index represents a letter
  int alphaLen = 26;
  int alpha[alphaLen];

  // ids are 26 characters + null
  char *temp = malloc(sizeof(char) * 27);
  while(fscanf(f, "%s\n", temp) != EOF) {
    // zero the array
    memset(alpha, 0, sizeof(alpha));

    // iterate through chars
    int len = strlen(temp), numTwos = 0, numThrees = 0;
    for(int i=0; i<len; i++) {
      // increment the character occurrence
      int charIdx = alphaIndex(temp[i]);
      alpha[charIdx] += 1;
      
      // adjust our local counts
      if(alpha[charIdx] == 2) {
        numTwos += 1;
      } else if (alpha[charIdx] == 3) {
        numTwos -= 1;
        numThrees += 1;
      } else if (alpha[charIdx] > 3) {
        numThrees -= 1;
      }
    }
    // if the local count has 1+ occurrence,
    // increment our global id counts:
    if (numTwos) twos += 1;
    if (numThrees) threes += 1;
  }

  free(temp);
  fclose(f);

  printf("%d\n", twos * threes);

  return 0;
}
