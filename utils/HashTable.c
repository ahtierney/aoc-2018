#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "HashTable.h"
#include "printhelpers.h"

static int INDEX_NOT_FOUND = -1;

void makeStorage(HashTable *table, size_t size);
bool compareKeys(EntryKeyType keyType, void *key, int keyLen, TableEntry *existing);

void performInsert(HashTable *table, EntryKeyType type, void *key, int keyLen, void *value, int valueLen);

int performLookUp(HashTable *table, EntryKeyType type, void *key, int keyLen);

int hash(void *key, int keyLen, EntryKeyType keyType, size_t size);
int hashInt(int *i, size_t size);
int hashString(char *str, int keyLen, size_t size);

TableEntry * newTableEntry() {
  TableEntry *entry = malloc(sizeof(TableEntry));
  entry->keyType = UNDEF;
  entry->key = NULL;
  entry->keyLen = 0;
  entry->value = NULL;
  entry->valueLen = 0;
  return entry;
}

void freeTableEntry(TableEntry *e) {
  free(e);
}

HashTable * newHashTable(size_t defaultSize) {
  HashTable *table = malloc(sizeof(HashTable));
  table->objs = 0;
  table->_size = 0;
  makeStorage(table, defaultSize);
  return table;
}

void freeHashTable(HashTable *table) {
  for (int i = 0; i<table->_size; i++) {
    if (table->_storage[i])
      freeTableEntry(table->_storage[i]);
      table->_storage[i] = 0;
  }
}

void makeStorage(HashTable *table, size_t size) {
  int sizes[27] = {0, 3, 6, 11, 19, 32, 52, 85, 139, 227, 373, 613, 1009, 1657, 2713, 4441,
    7283, 11923, 19531, 31973, 52361, 85717, 140321, 229711, 376039, 615577, 1007701};
  
  int allocSize;
  for (int i = 0; i<27; i++) {
    if (sizes[i] >= size) {
      allocSize = sizes[i];
    }
  }
  
  // can implement shrinking behavior for space efficiency here
  if (table->_size >= size) return;

  if (!table->_storage) {
    table->_storage = calloc(sizeof(void*), allocSize);
  } else {
    void **oldStorage = table->_storage;
    int oldSize = table->_size;

    table->_storage = calloc(sizeof(void*), allocSize);

    // can optimize here so we don't suffer this all in one go
    TableEntry *e;
    for (int i=0; i <oldSize; i++) {
      e = table->_storage[i];
      if (e)
        performInsert(table, e->keyType, e->key, e->keyLen, e->value, e->valueLen);
    }

    free(oldStorage);
  }
  table->_size = allocSize;
}

#pragma mark - insert

void insertIntoTableIntKey(HashTable *table, int *key, void *value, int valueLen) {
  performInsert(table, INT_TYPE, key, 1, value, valueLen);
};

void insertIntoTableStrKey(HashTable *table, char *key, int keyLen, void *value, int valueLen) {
  performInsert(table, STR_TYPE, key, keyLen, value, valueLen);
};

bool compareKeys(EntryKeyType keyType, void *key, int keyLen, TableEntry *existing) {
  switch (keyType) {
    case INT_TYPE:
      return (int*)key == existing->key;
    case STR_TYPE:
      return strcmp(key, existing->key) == 0;
    default:
      assert(false);
  }
  return false;
}

void performInsert(HashTable *table, EntryKeyType type, void *key, int keyLen, void *value, int valueLen) {
  // if table is emtpy make space:
  if (table->_size == 0) makeStorage(table, 1);
  // if the table is half full, expand the size and start again:
  else if (table->_size < table->objs * 1.3) makeStorage(table, table->_size + 1);

  int i = hash(key, keyLen, type, table->_size);
  while (table->_storage[i] != 0) {

    // if there is already an entry for the key
    if (table->_storage[i] && compareKeys(type, key, keyLen, table->_storage[i])) {
      ((TableEntry *) table->_storage[i])->value = value;
      break;
    }

    if (i == table->_size - 1) {
      i = 0;
      break;
    } else {
      i+=1;
    }
  }

  TableEntry *entry = newTableEntry();
  entry->keyType = type;
  entry->key = key;
  entry->keyLen = keyLen;
  entry->value = value;
  entry->valueLen = valueLen;

  table->_storage[i] = entry;
  table->objs++;
}

#pragma mark - lookup

TableEntry * valueFromTableIntKey(HashTable *table, int key) {
  int idx = performLookUp(table, INT_TYPE, &key, 1);
  if (idx == INDEX_NOT_FOUND) return NULL;
  return table->_storage[idx];
}

TableEntry * valueFromTableStrKey(HashTable *table, char *key, size_t keyLen) {
  int idx = performLookUp(table, STR_TYPE, key, keyLen);
  if (idx == INDEX_NOT_FOUND) return NULL;
  return table->_storage[idx];
}

#pragma mark - removal

void removeFromTableIntKey(HashTable *table, int key) {
  int idx = performLookUp(table, INT_TYPE, &key, 1);
  if (idx == INDEX_NOT_FOUND) return;
  freeTableEntry(table->_storage[idx]);
  table->_storage[idx] = 0;
  table->objs--;
};

void removeFromTableStrKey(HashTable *table, char *key, size_t keyLen) {
  int idx = performLookUp(table, STR_TYPE, key, keyLen);
  if (idx == INDEX_NOT_FOUND) return;
  freeTableEntry(table->_storage[idx]);
  table->_storage[idx] = 0;
  table->objs--;
};

int performLookUp(HashTable *table, EntryKeyType type, void *key, int keyLen) {
  int i = hash(key, keyLen, type, table->_size);
  // check indices for match until we hit a blank index
  while (table->_storage[i] != 0) {
    if (table->_storage[i] && compareKeys(type, key, keyLen, table->_storage[i])) {
      return i;
    }

    if (i == table->_size - 1) {
      i = 0;
      break;
    } else {
      i+=1;
    }
  }

  return INDEX_NOT_FOUND;
}

#pragma mark - hash

int hash(void *key, int keyLen, EntryKeyType keyType, size_t size) {
  switch(keyType) {
    case STR_TYPE:
      return hashString(key, keyLen, size);
    case INT_TYPE:
      return hashInt(key, size);
    default:
      assert(false);
  }
}

int hashInt(int *i, size_t size) {
  return *i % size;
}

int hashString(char *str, int keyLen, size_t size) {
  int hash = 0;
  for (int i = 0; i < keyLen; i++) {
      hash = (31 * hash + (int) str[i]) % size;
  }
  return hash;
}

void printTable(HashTable *table, void (*printKey)(void *, int), void (*printValue)(void *, int)) {
  for (int i = 0; i < table->_size; i++) {
    if (table->_storage[i]) {
      TableEntry *entry = table->_storage[i];
      printf("key: ");
      (*printKey)(entry->key, entry->keyLen);
      printf("  value: ");
      (*printKey)(entry->value, entry->valueLen);
      printf("\n");
    }
  }
}
