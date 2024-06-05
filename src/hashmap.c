#include <ptsd/hashmap.h>
#include <stdint.h>
#include <string.h>

HASHMAP_ERROR_TYPE HashMapInit(hashmap_t *map, size_t cap, size_t num_buckets,
			       size_t key_size, size_t value_size)
{
	if (cap == 0) {
		return HASHMAP_ERR_CAP_INVALID;
	}
	if (key_size == 0) {
		return HASHMAP_ERR_KEY_SIZE_INVALID;
	}
	if (value_size == 0) {
		return HASHMAP_ERR_VALUE_SIZE_INVALID;
	}

	map->cap = cap;
	map->size = 0;
	map->num_buckets = num_buckets;
	map->buckets = calloc(num_buckets * sizeof(node),
			      sizeof(key_size + value_size));

	map->key_size = key_size;
	map->value_size = value_size;
	return HASHMAP_SUCCESS;
}

uint32_t fnv1a_hash(uint8_t *key, size_t len)
{
	uint32_t FNV_offset_basis = 2166136261;
	uint32_t FNV_prime = 16777619;

	uint32_t hash_value = FNV_offset_basis;
	for (size_t i = 0; i < len; ++i) {
		hash_value ^= key[i];
		hash_value *= FNV_prime;
	}

	return hash_value;
}

HASHMAP_ERROR_TYPE HashMapGet(hashmap_t *map, void *key, void *value)
{
	uint32_t hash = fnv1a_hash(key, map->key_size);
	size_t index = hash % map->num_buckets;
	void **bucket = map->buckets + (index * sizeof(void *));
	node *current = *bucket;
	if (bucket == NULL) {
		return HASHMAP_ERR_KEY_NOT_FOUND;
	}

	while (current->next != NULL) {
		current = current->next;
		void *current_key = current + sizeof(node);
		if (memcmp(current_key, key, map->key_size) == 0) {
			break;
		}
	}

	memcpy(key, current + sizeof(node), map->key_size);
	memcpy(value, current + sizeof(node) + map->key_size, map->value_size);

	return HASHMAP_SUCCESS;
}

HASHMAP_ERROR_TYPE HashMapInsert(hashmap_t *map, void *key, void *value)
{
	uint32_t hash = fnv1a_hash(key, map->key_size);
	size_t index = hash % map->num_buckets;
	void **bucket = map->buckets + (index * sizeof(void *));
	node *new_node = malloc(sizeof(node) + map->key_size + map->value_size);
	if (*bucket == NULL) {
		*bucket = new_node;
	}
	node *current = *bucket;

	while (current->next != NULL) {
		current = current->next;
	}

	current->next = new_node;
	new_node->next = NULL;
	memcpy(new_node + sizeof(node), key, map->key_size);
	memcpy(new_node + sizeof(node) + map->key_size, value, map->value_size);

	map->size++;
	return HASHMAP_SUCCESS;
}

HASHMAP_ERROR_TYPE HashMapGrow(hashmap_t *map)
{
	size_t new_num_buckets = map->num_buckets * 2;
	void **new_buckets = calloc(new_num_buckets * sizeof(node),
				    sizeof(map->key_size + map->value_size));
	for (size_t i = 0; i < map->num_buckets; i++) {
		void **bucket = map->buckets + (i * sizeof(void *));
		node *current = *bucket;
		while (current->next != NULL) {
			uint32_t hash = fnv1a_hash(
				(void *)current + sizeof(node), map->key_size);

			size_t index = hash % new_num_buckets;
			void **new_bucket =
				new_buckets + (index * sizeof(void *));
			if (*new_bucket == NULL) {
				*new_bucket = current;
			}

			node *new_current = *new_bucket;

			while (new_current->next != NULL) {
				new_current = new_current->next;
			}
			new_current->next = current;
		}
	}
	free(map->buckets);
	map->buckets = new_buckets;
	map->num_buckets = new_num_buckets;
	return HASHMAP_SUCCESS;
}

HASHMAP_ERROR_TYPE HashMapShrink(hashmap_t *map)
{
	size_t new_num_buckets = map->num_buckets / 2;
	void **new_buckets = calloc(new_num_buckets * sizeof(node),
				    sizeof(map->key_size + map->value_size));
	for (size_t i = 0; i < map->num_buckets; i++) {
		void **bucket = map->buckets + (i * sizeof(void *));
		node *current = *bucket;
		while (current->next != NULL) {
			uint32_t hash = fnv1a_hash(
				(void *)current + sizeof(node), map->key_size);
			size_t index = hash % new_num_buckets;
			void **new_bucket =
				new_buckets + (index * sizeof(void *));
			if (*new_bucket == NULL) {
				*new_bucket = current;
			}
			node *new_current = *new_bucket;
			while (new_current->next != NULL) {
				new_current = new_current->next;
			}
			new_current->next = current;
		}
	}
	free(map->buckets);
	map->buckets = new_buckets;
	map->num_buckets = new_num_buckets;
	return HASHMAP_SUCCESS;
}
