
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ptsd/vector.h>

typedef uint8_t u8;

VECTOR_ERROR_TYPE VectorInit(struct Vector *vec, size_t cap, size_t item_size)
{
	if (cap != 0) {
		vec->cap = 10;
	}

	if (item_size == 0) {
		return VECTOR_ERR_SIZE_INVALID;
	}

	if (vec == NULL) {
		return VECTOR_ERR_INVALID_PTR;
	}

	vec->data = malloc(cap * item_size);
	vec->size = 0;
	vec->item_size = item_size;
	vec->cap = cap;

	return VECTOR_SUCCESS;
}

void *VectorGet(struct Vector *vec, int index)
{
	if (vec->size < index) {
		return NULL;
	}

	return (u8 *)vec->data + vec->item_size * index;
}

void VectorPush(struct Vector *vec, void *val)
{
	if (vec->cap == vec->size) {
		vec->cap *= REALLOC_SCALE;
		void *tmp = realloc(vec->data, vec->cap * vec->item_size);
		if (tmp == NULL) {
			tmp = malloc(vec->cap * vec->item_size);
			memcpy(tmp, vec->data, vec->size * vec->item_size);
			free(vec->data);
		}

		vec->data = tmp;
	}

	void *dest = (u8 *)vec->data + vec->item_size * vec->size;
	memcpy(dest, val, vec->item_size);

	vec->size++;
}

void VectorPop(struct Vector *vec)
{
	if (vec->size == 0)
		return;

	vec->size--;
	if (vec->size <= vec->cap / 4 && vec->cap > REALLOC_SCALE) {
		size_t new_size = vec->cap / REALLOC_SCALE;
		void *tmp = realloc(vec->data, new_size * vec->item_size);
		if (tmp == NULL) {
			tmp = malloc(vec->cap * vec->item_size);
			memcpy(tmp, vec->data, vec->size * vec->item_size);
			free(vec->data);
		}
		vec->data = tmp;
		vec->cap = new_size;
	}
}

size_t VectorLength(struct Vector *vec)
{
	return vec->size;
}
