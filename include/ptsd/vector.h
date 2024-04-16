#include <stdlib.h>

#ifndef REALLOC_SCALE
#define REALLOC_SCALE 2.0f
#endif

typedef enum {
	VECTOR_SUCCESS = 0,
	VECTOR_ERR_SIZE_INVALID_PTR,
	VECTOR_ERR_INVALID_PTR,
	VECTOR_ERR_INDEX_INVALID,
} VECTOR_ERROR_TYPE;

struct Vector {
	size_t cap;
	size_t size;
	size_t item_size;
	void *data;
};

VECTOR_ERROR_TYPE VectorInit(struct Vector *vec, size_t cap, size_t item_size);

void *VectorGet(struct Vector *vec, int index);

void VectorPush(struct Vector *vec, void *val);

void VectorPop(struct Vector *vec);

size_t VectorLength(struct Vector *vec);
