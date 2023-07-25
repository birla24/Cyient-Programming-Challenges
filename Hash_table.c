#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

typedef struct KeyValuePair {
    char* key;
    char* value;
    struct KeyValuePair* next;
} KeyValuePair;

typedef struct {
    int capacity;
    int size;
    KeyValuePair** buckets;
} HashTable;

unsigned int hash(const char* key, int capacity) {
    unsigned int hash_value = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash_value = (hash_value + key[i]) % capacity;
    }
    return hash_value;
}

KeyValuePair* createKeyValuePair(const char* key, const char* value) {
    KeyValuePair* kvp = (KeyValuePair*)malloc(sizeof(KeyValuePair));
    kvp->key = strdup(key);
    kvp->value = strdup(value);
    kvp->next = NULL;
    return kvp;
}

void freeKeyValuePair(KeyValuePair* kvp) {
    free(kvp->key);
    free(kvp->value);
    free(kvp);
}

HashTable* createHashTable(int capacity) {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->capacity = capacity;
    hashTable->size = 0;
    hashTable->buckets = (KeyValuePair**)calloc(capacity, sizeof(KeyValuePair*));
    return hashTable;
}

void put(HashTable* hashTable, const char* key, const char* value) {
    unsigned int index = hash(key, hashTable->capacity);
    KeyValuePair* current = hashTable->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = strdup(value);
            return;
        }
        current = current->next;
    }

    KeyValuePair* kvp = createKeyValuePair(key, value);
    kvp->next = hashTable->buckets[index];
    hashTable->buckets[index] = kvp;
    hashTable->size++;

    if (hashTable->size >= 0.7 * hashTable->capacity) {
        // Resize the hash table
        // Note: Implementing the resize logic is omitted in this example for simplicity.
        // In a real-world scenario, you would create a new larger hash table,
        // rehash all the elements, and free the old table.
    }
}

const char* get(HashTable* hashTable, const char* key) {
    unsigned int index = hash(key, hashTable->capacity);
    KeyValuePair* current = hashTable->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

void freeHashTable(HashTable* hashTable) {
    for (int i = 0; i < hashTable->capacity; i++) {
        KeyValuePair* current = hashTable->buckets[i];
        while (current) {
            KeyValuePair* next = current->next;
            freeKeyValuePair(current);
            current = next;
        }
    }
    free(hashTable->buckets);
    free(hashTable);
}

int main() {
    HashTable* hashTable = createHashTable(INITIAL_CAPACITY);
    put(hashTable, "name", "John");
    put(hashTable, "age", "30");
    put(hashTable, "city", "New York");

    // O(1) run time for value look up
    printf("Name: %s\n", get(hashTable, "name")); // Output: John
    printf("Age: %s\n", get(hashTable, "age"));   // Output: 30
    printf("City: %s\n", get(hashTable, "city")); // Output: New York

    // Free the memory used by the hash table
    freeHashTable(hashTable);

    return 0;
}
