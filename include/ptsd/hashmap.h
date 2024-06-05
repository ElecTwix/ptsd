
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>

#ifndef REALLOC_SCALE
#define REALLOC_SCALE 2.0f
#endif

typedef enum {
	HASHMAP_SUCCESS = 0,
	HASHMAP_ERR_SIZE_INVALID,
	HASHMAP_ERR_CAP_INVALID,
	HASHMAP_ERR_KEY_SIZE_INVALID,
	HASHMAP_ERR_VALUE_SIZE_INVALID,
	HASHMAP_ERR_KEY_EXISTS,
	HASHMAP_ERR_KEY_NOT_FOUND
} HASHMAP_ERROR_TYPE;

typedef struct {
	void *next;
} node;

typedef struct {
	size_t cap;
	size_t size;

	size_t key_size;
	size_t value_size;

	size_t num_buckets;
	void *buckets;

} hashmap_t;

HASHMAP_ERROR_TYPE HashMapInit(hashmap_t *map, size_t cap, size_t num_buckets,
			       size_t key_size, size_t value_size);

HASHMAP_ERROR_TYPE HashMapGet(hashmap_t *map, void *key, void *value);

HASHMAP_ERROR_TYPE HashMapInsert(hashmap_t *map, void *key, void *value);

HASHMAP_ERROR_TYPE HashMapGrow(hashmap_t *map);

HASHMAP_ERROR_TYPE HashMapShrink(hashmap_t *map);

#define HASHMAP_H

#endif
