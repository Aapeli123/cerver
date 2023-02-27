#include "hashmap.h"

hashmap_t *create_hashmap(int initial_buckets)
{
    hashmap_t *map = calloc(1, sizeof(hashmap_t));
    bucket_t **buckets = (bucket_t **)calloc(initial_buckets, sizeof(bucket_t *));

    map->buckets = buckets;
    map->bucket_count = initial_buckets;

    return map;
}

// Destroys bucket and all following buckets in its linked list
static void destroy_bucket(bucket_t *bucket)
{
    if (bucket->next != NULL)
    {
        destroy_bucket(bucket->next);
    }

    free(bucket->key);
    free(bucket->value);
    free(bucket);
}

static void rehash(hashmap_t *hashmap, int new_bucket_count)
{
    // Do nothing if bucket count would decrease as we do not allow such things
    if (new_bucket_count <= hashmap->bucket_count)
    {
        return;
    }
    // Copy the bucket array to a new location in memory:
    bucket_t **temp_buckets = (bucket_t **)calloc(hashmap->bucket_count, sizeof(bucket_t *));
    memcpy(temp_buckets, hashmap->buckets, hashmap->bucket_count * sizeof(bucket_t *));
    int old_bucket_count = hashmap->bucket_count;

    // Allocate more memory for hashmap buckets and zero it:
    hashmap->buckets = realloc(hashmap->buckets, new_bucket_count);

    for (int i = 0; i < old_bucket_count; i++)
    {
        if (temp_buckets[i] == NULL)
            continue;

        int new_hash = hash(temp_buckets[i]->key, new_bucket_count);
    }
}

void hashmap_add(hashmap_t *hashmap, char *key, char *value)
{
    int index = hash(key, hashmap->bucket_count);

    bucket_t *bucket = calloc(1, sizeof(bucket_t));
    bucket->key = key;
    bucket->value = value;
    if (hashmap->buckets[index]->key != NULL && hashmap->buckets[index]->key != key) // Deal with a possible hash collision
    {
        bucket->next = hashmap->buckets[index];
    }

    hashmap->buckets[index] = bucket;
}

void hashmap_remove(hashmap_t *hashmap, char *key)
{
    int index = hash(key, hashmap->bucket_count);
    bucket_t *bucket = hashmap->buckets[index];
    bucket_t *prev = bucket;
    bool isFirst = true;
    while (bucket->key != key)
    {
        isFirst = false;
        if (bucket->next == NULL)
        {
            // If this happens I'm doomed:
            printf("Something did not work as intended.\n");
            return;
        }
        prev = bucket;
        bucket = bucket->next;
    }
    if (isFirst)
    {
        hashmap->buckets[index] = bucket->next;
    }
    prev->next = bucket->next;
    bucket->next = NULL;
    destroy_bucket(bucket);
}

char *hashmap_get(hashmap_t *hashmap, char *key)
{
    int index = hash(key, hashmap->bucket_count);
    bucket_t *b = hashmap->buckets[index];
    while (b->key != key)
    {
        if (b->next == NULL)
        {
            return NULL; // WTF
        }
        b = b->next;
    }
    return b->value;
}

void hashmap_destroy(hashmap_t *hashmap)
{
    for (int i = 0; i < hashmap->bucket_count; i++)
    {
        destroy_bucket(hashmap->buckets[i]);
    }
    free(hashmap->buckets);
    free(hashmap);
}