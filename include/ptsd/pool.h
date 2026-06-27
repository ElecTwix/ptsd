#ifndef POOL_H
#define POOL_H

#include <stdlib.h>
#include <stdint.h>

typedef struct {
	void *mem;
	size_t slot_size;
	uint32_t slot_amount;
	uint32_t *free_list_arr;
	uint32_t free_top;
} pool_t;

typedef enum {
	POOL_SUCCESS = 0,
	POOL_ERR_SIZE_INVALID,
	POOL_ERR_AMOUNT_INVALID,
	POOL_ERR_INVALID_PTR,
	POOL_ERR_INDEX_INVALID,
	POOL_ERR_EXHAUSTED,
	POOL_ERR_NOMEM,
} pool_status_t;

pool_status_t PoolInit(pool_t *pool, size_t slot_size, int slot_amount);
pool_status_t PoolAlloc(pool_t *pool, void **ptr, size_t size);
pool_status_t PoolClear(pool_t *pool, void *ptr);
pool_status_t PoolFree(pool_t *pool);

#endif
