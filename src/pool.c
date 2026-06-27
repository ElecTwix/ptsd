#include <ptsd/pool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#define POOL_ALIGN 16

static size_t round_up(size_t n, size_t a)
{
	return (n + a - 1) & ~(a - 1);
}

static size_t slots_bytes(const pool_t *pool)
{
	return pool->slot_amount * pool->slot_size;
}

static size_t total_bytes(const pool_t *pool)
{
	return pool->slot_amount * (pool->slot_size + sizeof(uint32_t));
}

pool_status_t PoolInit(pool_t *pool, size_t slot_size, int slot_amount)
{
	if (slot_size == 0) {
		return POOL_ERR_SIZE_INVALID;
	}
	if (slot_amount <= 0) {
		return POOL_ERR_AMOUNT_INVALID;
	}

	pool->slot_size = round_up(slot_size, POOL_ALIGN);
	pool->slot_amount = (uint32_t)slot_amount;

	void *p = mmap(0, total_bytes(pool), PROT_READ | PROT_WRITE,
		       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (p == MAP_FAILED) {
		return POOL_ERR_NOMEM;
	}

	pool->mem = p;
	pool->free_list_arr = (uint32_t *)((char *)p + slots_bytes(pool));

	uint32_t *arr = pool->free_list_arr;
	for (uint32_t i = 0; i < pool->slot_amount; i++) {
		arr[i] = i;
	}
	pool->free_top = pool->slot_amount;

	return POOL_SUCCESS;
}

pool_status_t PoolAlloc(pool_t *pool, void **ptr, size_t size)
{
	if (size > pool->slot_size) {
		return POOL_ERR_SIZE_INVALID;
	}
	if (pool->free_top == 0) {
		return POOL_ERR_EXHAUSTED;
	}

	uint32_t idx = pool->free_list_arr[--pool->free_top];
	*ptr = (char *)pool->mem + idx * pool->slot_size;
	return POOL_SUCCESS;
}

pool_status_t PoolClear(pool_t *pool, void *ptr)
{
	if (ptr == NULL) {
		return POOL_ERR_INVALID_PTR;
	}

	char *base = (char *)pool->mem;
	char *cp = (char *)ptr;

	if (cp < base || cp >= base + slots_bytes(pool)) {
		return POOL_ERR_INVALID_PTR;
	}
	size_t offset = (size_t)(cp - base);
	if (offset % pool->slot_size != 0) {
		return POOL_ERR_INVALID_PTR;
	}

	if (pool->free_top >= pool->slot_amount) {
		return POOL_ERR_INDEX_INVALID;
	}

	uint32_t idx = (uint32_t)(offset / pool->slot_size);
	pool->free_list_arr[pool->free_top++] = idx;
	return POOL_SUCCESS;
}

pool_status_t PoolFree(pool_t *pool)
{
	munmap(pool->mem, total_bytes(pool));
	pool->mem = NULL;
	pool->free_top = 0;
	return POOL_SUCCESS;
}
