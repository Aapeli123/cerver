#ifndef HASHMAP_H
#define HASHMAP_H
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "hash.h"
#include "primes.h"

struct Bucket
{
    char *key;
    void* value;
    int val_size;
    struct Bucket *next;
};

typedef struct Bucket bucket_t;

// Very simple implementation of a hash map with a string key and a string value
struct HashMap
{
    int bucket_count;
    int value_count;
    bucket_t **buckets;
};

typedef struct HashMap hashmap_t;

hashmap_t *hashmap_create();
void hashmap_add(hashmap_t *hashmap, char *key, void *value, int val_size);
void hashmap_remove(hashmap_t *hashmap, char *key);
void*hashmap_get(hashmap_t *hashmap, char *key);
void hashmap_destroy(hashmap_t *hashmap);

#define HASHMAP_ADD_STRING(hashmap, key, string) hashmap_add(hashmap, key, string, strlen(string) + 1)

#endif