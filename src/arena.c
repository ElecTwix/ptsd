#include <ptsd/arena.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

arena_status_t ArenaInit(arena_t *arena, size_t size)
{
	if (size == 0) {
		return ARENA_ERR_SIZE_INVALID;
	}

	chunk_t *chunk = (chunk_t *)malloc(sizeof(chunk_t));

	void *p = mmap(0, size, PROT_READ | PROT_WRITE,
		       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	chunk->mem = p;
	chunk->previous = NULL;
	chunk->used = 0;

	arena->size = size;
	arena->chunk = chunk;
	return ARENA_SUCCESS;
}

arena_status_t ArenaAlloc(arena_t *arena, void **ptr, size_t size)
{
	if (size == 0 || size > arena->size) {
		return ARENA_ERR_SIZE_INVALID;
	}

	if (arena->chunk->used + size > arena->size) {
		chunk_t *chunk = (chunk_t *)malloc(sizeof(chunk_t));
		void *p = mmap(0, size, PROT_READ | PROT_WRITE,
			       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		chunk->mem = p;
		chunk->previous = arena->chunk;
		arena->chunk = chunk;
	}

	*ptr = arena->chunk->mem + arena->chunk->used;
	arena->chunk->used += size;
	return ARENA_SUCCESS;
}

arena_status_t ArenaClear(arena_t *arena, void *ptr)
{
	arena->chunk->used -= sizeof(ptr);
	return ARENA_SUCCESS;
}

arena_status_t ArenaFree(arena_t *arena)
{
	while (arena->chunk != NULL) {
		chunk_t *next = arena->chunk->previous;
		munmap(arena->chunk->mem, arena->size);
		free(arena->chunk);
		arena->chunk = next;
	}

	return ARENA_SUCCESS;
}
