#include <CUnit/CUError.h>
#include <CUnit/TestRun.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <ptsd/vector.h>
#include <stdlib.h>

void test_vector_create(void)
{
	struct Vector vec;
	int a = VectorInit(&vec, 10, sizeof(int));
	CU_ASSERT_EQUAL(a, VECTOR_SUCCESS);
	CU_ASSERT_PTR_NOT_NULL(&vec);
	CU_ASSERT_EQUAL(vec.size, 0);
	CU_ASSERT_EQUAL(vec.cap, 10);
	CU_ASSERT_PTR_NOT_NULL(vec.data);
	CU_PASS("passed test_vector_create");
}

void test_vector_push(void)
{
	struct Vector vec;
	int a = VectorInit(&vec, 10, sizeof(int));
	CU_ASSERT_EQUAL(a, VECTOR_SUCCESS);
	int b = 10;
	VectorPush(&vec, &b);
	CU_ASSERT_EQUAL(vec.size, 1);
	CU_ASSERT_PTR_NOT_NULL(vec.data);
	CU_PASS("passed test_vector_push");
}

void test_vector_pop(void)
{
	struct Vector vec;
	int a = VectorInit(&vec, 10, sizeof(int));
	CU_ASSERT_EQUAL(a, VECTOR_SUCCESS);
	int b = 10;
	VectorPush(&vec, &b);
	CU_ASSERT_EQUAL(vec.size, 1);
	VectorPop(&vec);
	CU_ASSERT_EQUAL(vec.size, 0);
	CU_ASSERT_PTR_NOT_NULL(vec.data);
	CU_PASS("passed test_vector_pop");
}

void test_vector_get(void)
{
	struct Vector vec;
	int a = VectorInit(&vec, 10, sizeof(int));
	CU_ASSERT_EQUAL(a, VECTOR_SUCCESS);
	int b = 10;
	VectorPush(&vec, &b);
	int *c = VectorGet(&vec, 0);
	CU_ASSERT_EQUAL(*c, 10);
	CU_ASSERT_EQUAL(vec.size, 1);
	CU_ASSERT_EQUAL(vec.cap, 10);
	CU_ASSERT_PTR_NOT_NULL(vec.data);
	CU_PASS("passed test_vector_get");
}

void test_vector_resize_up(void)
{
	struct Vector vec;
	int a = VectorInit(&vec, 10, sizeof(int));
	CU_ASSERT_EQUAL(a, VECTOR_SUCCESS);
	for (int i = 0; i < 11; i++) {
		VectorPush(&vec, &i);
	}
	CU_ASSERT_EQUAL(vec.size, 11);
	CU_ASSERT_EQUAL(vec.cap, 20);
	CU_ASSERT_PTR_NOT_NULL(vec.data);
	CU_PASS("passed test_vector_resize");
}

void test_vector_resize_down(void)
{
	struct Vector vec;
	int a = VectorInit(&vec, 10, sizeof(int));
	CU_ASSERT_EQUAL(a, VECTOR_SUCCESS);
	for (int i = 0; i < 11; i++) {
		VectorPush(&vec, &i);
	}
	for (int i = 0; i < 11; i++) {
		VectorPop(&vec);
	}
	CU_ASSERT_EQUAL(vec.size, 0);
	CU_ASSERT(vec.cap <= 10)
	CU_ASSERT_PTR_NOT_NULL(vec.data);
	CU_PASS("passed test_vector_resize_down");
}

void test_vector_get_length(void)
{
	struct Vector vec;
	int a = VectorInit(&vec, 10, sizeof(int));
	CU_ASSERT_EQUAL(a, VECTOR_SUCCESS);
	for (int i = 0; i < 11; i++) {
		VectorPush(&vec, &i);
	}
	CU_ASSERT_EQUAL(VectorLength(&vec), 11);
	CU_PASS("passed test_vector_get_length");
}

int main(void)
{
	CU_initialize_registry();
	CU_pSuite suite = CU_add_suite("vector", NULL, NULL);

	CU_add_test(suite, "test_vector_create", test_vector_create);
	CU_add_test(suite, "test_vector_push", test_vector_push);
	CU_add_test(suite, "test_vector_pop", test_vector_pop);
	CU_add_test(suite, "test_vector_get", test_vector_get);

	CU_add_test(suite, "test_vector_resize_up", test_vector_resize_up);
	CU_add_test(suite, "test_vector_resize_down", test_vector_resize_down);
	CU_add_test(suite, "test_vector_get_length", test_vector_get_length);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_basic_show_failures(CU_get_failure_list());

	if (CU_get_number_of_tests_failed() > 0) {
		CU_cleanup_registry();
		return 1;
	}

	return 0;
}
