
#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#endif

typedef struct {
	void *mem;
	size_t used;
	void *previous;
} chunk_t;

typedef struct {
	size_t size;
	chunk_t *chunk;
} arena_t;

typedef enum {
	ARENA_SUCCESS = 0,
	ARENA_ERR_SIZE_INVALID,
	ARENA_ERR_INVALID_PTR,
	ARENA_ERR_INDEX_INVALID,
} arena_status_t;

arena_status_t ArenaInit(arena_t *arena, size_t size);

arena_status_t ArenaAlloc(arena_t *arena, void **ptr, size_t size);

arena_status_t ArenaClear(arena_t *arena, void *ptr);

arena_status_t ArenaFree(arena_t *arena);
