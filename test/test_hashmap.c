
#include <CUnit/CUError.h>
#include <CUnit/TestRun.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <ptsd/hashmap.h>
#include <stdlib.h>

void test_hashmap_create(void)
{
	hashmap_t map;
	size_t cap = 30;
	size_t num_buckets = 30;
	size_t keysize = sizeof(int);
	size_t valsize = sizeof(int);
	HASHMAP_ERROR_TYPE status =
		HashMapInit(&map, cap, num_buckets, keysize, valsize);
	CU_ASSERT_PTR_NOT_NULL(&map);

	CU_ASSERT_EQUAL(map.size, 0)
	CU_ASSERT_EQUAL(map.cap, cap)
	CU_ASSERT_EQUAL(map.num_buckets, num_buckets)
	CU_ASSERT_EQUAL(map.key_size, keysize)
	CU_ASSERT_EQUAL(map.value_size, valsize)
	CU_PASS("passed test_hashmap_create");
}

hashmap_t helper_hashmap_create(void)
{
	hashmap_t map;
	size_t cap = 30;
	size_t num_buckets = 30;
	size_t keysize = sizeof(int);
	size_t valsize = sizeof(int);
	HASHMAP_ERROR_TYPE status =
		HashMapInit(&map, cap, num_buckets, keysize, valsize);
	return map;
}

void test_hashmap_set(void)
{
	hashmap_t map = helper_hashmap_create();
	int key = 10;
	int value = 20;
	HASHMAP_ERROR_TYPE status = HashMapInsert(&map, &key, &value);
	CU_ASSERT_EQUAL(map.size, 1);
	CU_PASS("passed test_hashmap_push");
}

void test_hashmap_get(void)
{
	hashmap_t map = helper_hashmap_create();
	int key = 10;
	HASHMAP_ERROR_TYPE status;

	for (int i = 0; i < 100; i++) {
		status = HashMapInsert(&map, &key, &i);
		CU_ASSERT_EQUAL(status, HASHMAP_SUCCESS);

		int *new_val = malloc(sizeof(int));
		status = HashMapGet(&map, &key, new_val);
		CU_ASSERT_EQUAL(status, HASHMAP_SUCCESS);
		CU_ASSERT_EQUAL(*new_val, i);
	}

	CU_PASS("passed test_get");
}

void test_hashmap_grow(void)
{
	hashmap_t map = helper_hashmap_create();
	int key = 10;
	HASHMAP_ERROR_TYPE status;
	for (int i = 0; i < 100; i++) {
		status = HashMapInsert(&map, &key, &i);
		CU_ASSERT_EQUAL(status, HASHMAP_SUCCESS);
	}
	CU_ASSERT_EQUAL(map.size, 100);
	CU_ASSERT_EQUAL(map.num_buckets, 30);
	HashMapGrow(&map);
	CU_ASSERT_EQUAL(map.num_buckets, 60);
	for (int i = 0; i < 100; i++) {
		int *get_key = malloc(sizeof(int));
		int *get_val = malloc(sizeof(int));
		status = HashMapGet(&map, &key, &i);
		CU_ASSERT_EQUAL(status, HASHMAP_SUCCESS);
		CU_ASSERT_EQUAL(key, get_key);
		CU_ASSERT_EQUAL(i, get_val);
	}

	CU_PASS("passed test_growth");
}

void test_hashmap_shrink(void)
{
	hashmap_t map = helper_hashmap_create();
	int key = 10;
	HASHMAP_ERROR_TYPE status;
	for (int i = 0; i < 100; i++) {
		status = HashMapInsert(&map, &key, &i);
		CU_ASSERT_EQUAL(status, HASHMAP_SUCCESS);
	}
	CU_ASSERT_EQUAL(map.size, 100);
	CU_ASSERT_EQUAL(map.num_buckets, 30);
	HashMapShrink(&map);
	CU_ASSERT_EQUAL(map.num_buckets, 60);
	for (int i = 0; i < 100; i++) {
		int *get_key = malloc(sizeof(int));
		int *get_val = malloc(sizeof(int));
		status = HashMapGet(&map, &key, &i);
		CU_ASSERT_EQUAL(status, HASHMAP_SUCCESS);
		CU_ASSERT_EQUAL(key, get_key);
		CU_ASSERT_EQUAL(i, get_val);
	}

	CU_PASS("passed test_shrink");
}

int main(void)
{
	CU_initialize_registry();

	CU_pSuite suite = CU_add_suite("hashmap", NULL, NULL);

	CU_add_test(suite, "test_hashmap_create", test_hashmap_create);
	CU_add_test(suite, "test_hashmap_set", test_hashmap_set);
	CU_add_test(suite, "test_hashmap_get", test_hashmap_get);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_basic_show_failures(CU_get_failure_list());

	if (CU_get_number_of_tests_failed() > 0) {
		CU_cleanup_registry();
		return 1;
	}

	return 0;
}
