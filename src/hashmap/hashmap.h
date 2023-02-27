#ifndef HASHMAP_H
#define HASHMAP_H
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

struct Bucket
{
    char *key;
    char *value;
    bucket_t *next;
};

typedef struct Bucket bucket_t;

struct HashMap
{
    int bucket_count;
    bucket_t **buckets;
};

typedef struct HashMap hashmap_t;
#endif