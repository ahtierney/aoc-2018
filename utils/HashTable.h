#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef enum EntryKeyType EntryKeyType;
enum EntryKeyType {
  UNDEF = 0,
  INT_TYPE = (1 << 0),
  STR_TYPE = (1 << 2),
};

typedef struct TableEntry TableEntry;
struct TableEntry {
  EntryKeyType keyType;
  void *key;
  int keyLen;
  void *value;
  int valueLen;
};

typedef struct HashTable HashTable;
struct HashTable {
  int objs;
  int _size;
  void **_storage;
};

HashTable * newHashTable(size_t defaultSize);
void freeHashTable(HashTable *table);

void insertIntoTableIntKey(HashTable *table, int *key, void *value, int valueLen);
void insertIntoTableStrKey(HashTable *table, char *key, int keyLen, void *value, int valueLen);

TableEntry * valueFromTableIntKey(HashTable *table, int key);
TableEntry * valueFromTableStrKey(HashTable *table, char *key, size_t keyLen);

void removeFromTable(HashTable *table, void *key, void *value);

void printTable(HashTable *table, void (*printKey)(void *, int), void (*printValue)(void *, int));

#endif
