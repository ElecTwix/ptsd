#include <CUnit/CUError.h>
#include <CUnit/TestRun.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <ptsd/arena.h>
#include <stdlib.h>

void test_arena_create(void)
{
	arena_t arena;
	size_t size = 30;

	arena_status_t status = ArenaInit(&arena, size);
	if (status != ARENA_SUCCESS) {
		CU_FAIL("failed test_arena_alloc");
	}
	CU_ASSERT_PTR_NOT_NULL(&arena);

	CU_ASSERT_EQUAL(arena.size, size)
	CU_ASSERT_PTR_NOT_NULL(arena.chunk);
	CU_PASS("passed test_arena_create");
}

arena_t helper_arena_create(size_t size)
{
	arena_t arena;

	arena_status_t status = ArenaInit(&arena, size);
	return arena;
}

void test_arena_alloc(void)
{
	arena_t arena = helper_arena_create(30);
	int value = 20;
	int value2 = 30;
	size_t size = sizeof(value) + sizeof(value2);
	void *ptr;

	if (arena.chunk->used != 0) {
		CU_FAIL("failed test_arena_alloc");
	}

	arena_status_t status = ArenaAlloc(&arena, &ptr, size);
	if (status != ARENA_SUCCESS) {
		CU_FAIL("failed test_arena_alloc");
	}

	if (arena.chunk->used != size) {
		CU_FAIL("failed test_arena_alloc");
	}

	// set the value at the pointer
	*(int *)ptr = value;
	*(int *)(ptr + sizeof(value)) = value2;

	if (*(int *)ptr != value) {
		CU_FAIL("failed test_arena_alloc");
	}
	if (*(int *)(ptr + sizeof(value)) != value2) {
		CU_FAIL("failed test_arena_alloc");
	}

	CU_ASSERT_EQUAL(arena.size, 30);
	CU_PASS("passed test_arena_alloc")
}

void test_arena_over_alloc(void)
{
	arena_t arena = helper_arena_create(sizeof(int) * 2);
	int value = 20;
	int value2 = 30;
	int value3 = 30;
	size_t size = sizeof(value) + sizeof(value2);
	void *ptr;

	arena_status_t status = ArenaAlloc(&arena, &ptr, size);
	if (status != ARENA_SUCCESS) {
		CU_FAIL("failed test_arena_alloc");
	}

	void *first_chunk = arena.chunk;
	if (arena.chunk->used != size) {
		CU_FAIL("failed test_arena_alloc");
	}
	// set the value at the pointer
	*(int *)ptr = value;
	*(int *)(ptr + sizeof(value)) = value2;

	if (*(int *)ptr != value) {
		CU_FAIL("failed test_arena_alloc");
	}
	if (*(int *)(ptr + sizeof(value)) != value2) {
		CU_FAIL("failed test_arena_alloc");
	}

	status = ArenaAlloc(&arena, &ptr, size);
	if (status != ARENA_SUCCESS) {
		CU_FAIL("failed test_arena_alloc");
	}

	if (arena.chunk->used != size) {
		CU_FAIL("failed test_arena_alloc");
	}
	void *newChunk = arena.chunk;
	if (newChunk == first_chunk) {
		CU_FAIL("failed test_arena_alloc");
	}

	*(int *)ptr = value3;
	if (*(int *)ptr != value3) {
		CU_FAIL("failed test_arena_alloc");
	}

	CU_PASS("passed test_arena_over_alloc");
}

void test_arena_clear(void)
{
	arena_t arena = helper_arena_create(30);
	int value = 20;
	size_t size = sizeof(value);
	void *ptr;
	arena_status_t status = ArenaAlloc(&arena, &ptr, size);
	if (status != ARENA_SUCCESS) {
		CU_FAIL("failed test_arena_alloc");
	}
	if (arena.chunk->used != size) {
		CU_FAIL("failed test_arena_alloc");
	}
	// set the value at the pointer
	*(int *)ptr = value;
	if (*(int *)ptr != value) {
		CU_FAIL("failed test_arena_alloc");
	}
	status = ArenaClear(&arena, ptr);
	if (status != ARENA_SUCCESS) {
		CU_FAIL("failed test_arena_free");
	}
	if (arena.chunk->used != 0) {
		CU_FAIL("failed test_arena_free");
	}
	CU_PASS("passed test_arena_free");
}

void test_arena_free(void)
{
	arena_t arena = helper_arena_create(30);
	int value = 20;
	size_t size = sizeof(value);
	void *ptr;
	arena_status_t status = ArenaAlloc(&arena, &ptr, size);
	if (status != ARENA_SUCCESS) {
		CU_FAIL("failed test_arena_alloc");
	}
	if (arena.chunk->used != size) {
		CU_FAIL("failed test_arena_alloc");
	}
	// set the value at the pointer
	*(int *)ptr = value;
	if (*(int *)ptr != value) {
		CU_FAIL("failed test_arena_alloc");
	}
	status = ArenaFree(&arena);
	if (status != ARENA_SUCCESS) {
		CU_FAIL("failed test_arena_free");
	}
	if (arena.chunk != NULL) {
		CU_FAIL("failed test_arena_free");
	}
	CU_PASS("passed test_arena_free");
}

int main(void)
{
	CU_initialize_registry();

	CU_pSuite suite = CU_add_suite("arena", NULL, NULL);

	CU_add_test(suite, "test_arena_create", test_arena_create);
	CU_add_test(suite, "test_arena_create", test_arena_alloc);
	CU_add_test(suite, "test_arena_create", test_arena_over_alloc);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_basic_show_failures(CU_get_failure_list());

	if (CU_get_number_of_tests_failed() > 0) {
		CU_cleanup_registry();
		return 1;
	}

	return 0;
}
