#include <stdio.h>
#include "ccvector.h"
#include "ccstack.h"
#include "cchashtable.h"
#include "ccheap.h"
#include "cctree.h"

int TestVector();
int TestStack();
int TestHashTable();
int TestHeap();
int TestTree();

void RunTests();

int main(void)
{
	RunTests();
	return 0;
}

void RunTests()
{
	/// NOTE: The tests provided here are by no means exhaustive and are only
	/// provided as a starting point (not all functions are tested, not all use cases
	/// and failure scenarios are covered). You are encouraged to expand these tests
	/// to include missing scenarios.
	if (0 == TestVector())
	{
		printf("Vector test passed\n\n");
	}
	else
	{
		printf("Vector test failed\n\n");
	}

	if (0 == TestStack())
	{
		printf("Stack test passed\n\n");
	}
	else
	{
		printf("Stack test failed\n\n");
	}

	if (0 == TestHashTable())
	{
		printf("HashTable test passed\n\n");
	}
	else
	{
		printf("HashTable test failed\n\n");
	}

	if (0 == TestHeap())
	{
		printf("Heap test passed\n\n");
	}
	else
	{
		printf("Heap test failed\n\n");
	}

	if (0 == TestTree())
	{
		printf("Tree test passed\n\n");
	}
	else
	{
		printf("Tree test failed\n\n");
	}
}


int TestTree()
{
	int retVal = -1;
	CC_TREE* usedTree = NULL;

	retVal = TreeCreate(&usedTree);
	if (0 != retVal)
	{
		printf("TreeCreate failed!\n");
		goto cleanup;
	}

	retVal = TreeInsert(usedTree, 20);
	if (0 != retVal)
	{
		printf("TreeInsert failed!\n");
		goto cleanup;
	}

	if (1 != TreeContains(usedTree, 20))
	{
		printf("TreeContains invalid return value!\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = TreeRemove(usedTree, 20);
	if (0 != retVal)
	{
		printf("TreeRemove failed!\n");
		goto cleanup;
	}

	if (0 != TreeContains(usedTree, 20))
	{
		printf("TreeContains invalid return value after remove!\n");
		retVal = -1;
		goto cleanup;
	}

	if (0 != TreeGetCount(usedTree))
	{
		printf("TreeGetCount invalid return value!\n");
		retVal = -1;
		goto cleanup;
	}

cleanup:
	if (NULL != usedTree)
	{
		if (0 != TreeDestroy(&usedTree))
		{
			printf("TreeDestroy failed!\n");
			retVal = -1;
		}
	}
	return retVal;
}

int TestHeap()
{
	int retVal = -1;
	int foundVal = -1;
	CC_HEAP* usedHeap = NULL;
	CC_VECTOR* usedVector = NULL;
	VecCreate(&usedVector);
	VecInsertTail(usedVector, 4); VecInsertTail(usedVector, 1); VecInsertTail(usedVector, 3);
	VecInsertTail(usedVector, 10); VecInsertTail(usedVector, 14);

	retVal = HpCreateMinHeap(&usedHeap, usedVector);
	if (0 != retVal)
	{
		printf("HpCreateMinHeap failed!\n");
		goto cleanup;
	}
	/*printf("Initial Heap:  ");
	for (int i = 0; i < usedHeap->Size; ++i)
	{
		printf("%d ", usedHeap->Array[i]);
	}
	printf("\n");*/

	retVal = HpInsert(usedHeap, 20);
	if (0 != retVal)
	{
		printf("HpInsert failed!\n");
		goto cleanup;
	}
	retVal = HpInsert(usedHeap, 10);
	if (0 != retVal)
	{
		printf("HpInsert failed!\n");
		goto cleanup;
	}
	retVal = HpInsert(usedHeap, 6);
	if (0 != retVal)
	{
		printf("HpInsert failed!\n");
		goto cleanup;
	}
	retVal = HpInsert(usedHeap, 9);
	if (0 != retVal)
	{
		printf("HpInsert failed!\n");
		goto cleanup;
	}
	retVal = HpInsert(usedHeap, 1);
	if (0 != retVal)
	{
		printf("HpInsert failed!\n");
		goto cleanup;
	}
	retVal = HpInsert(usedHeap, 3);
	if (0 != retVal)
	{
		printf("HpInsert failed!\n");
		goto cleanup;
	}

	/*CC_VECTOR* vector;
	VecCreate(&vector);
	retVal = HpSortToVector(usedHeap, vector);
	printf("Sorted vector:  ");
	for (int i = 0; i < vector->Count; ++i)
	{
		printf("%d ", vector->Array[i]);
	}*/
	
	printf("\nFinal Heap:  ");
	for (int i = 0; i < usedHeap->Count; ++i)
	{
		printf("%d ", usedHeap->Array[i]);
	}
	printf("\n");

	if (11 != HpGetElementCount(usedHeap))
	{
		printf("Invalid element count!\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = HpGetExtreme(usedHeap, &foundVal);
	if (0 != retVal)
	{
		printf("HpGetExtreme failed!\n");
		goto cleanup;
	}

	if (1 != foundVal)
	{
		printf("Invalid minimum value returned!\n");
		retVal = -1;
		goto cleanup;
	}

cleanup:
	if (NULL != usedHeap)
	{
		if (0 != HpDestroy(&usedHeap))
		{
			printf("HpDestroy failed!\n");
			retVal = -1;
		}
	}
	return retVal;
}

int TestHashTable()
{
	int retVal = -1;
	int foundVal = -1;
	CC_HASH_TABLE* usedTable = NULL;

	retVal = HtCreate(&usedTable);
	if (0 != retVal)
	{
		printf("HtCreate failed!\n");
		goto cleanup;
	}

	retVal = HtSetKeyValue(usedTable, "mere", 20);
	if (0 != retVal)
	{
		printf("HtSetKeyValue failed!\n");
		goto cleanup;
	}

	retVal = HtSetKeyValue(usedTable, "maria", 10);
	if (0 != retVal)
	{
		printf("HtSetKeyValue failed!\n");
		goto cleanup;
	}

	if (1 != HtHasKey(usedTable, "mere"))
	{
		printf("Invalid answer to HtHasKey!\n");
		retVal = -1;
		goto cleanup;
	}

	if (1 != HtHasKey(usedTable, "maria"))
	{
		printf("Invalid answer to HtHasKey!\n");
		retVal = -1;
		goto cleanup;
	}

	if (0 != HtRemoveKey(usedTable, "maria"))
	{
		printf("Remove has failed!\n");
		retVal = -1;
		goto cleanup;
	}

	if (0 != HtHasKey(usedTable, "maria"))
	{
		printf("Key wasn't removed properly!\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = HtGetKeyValue(usedTable, "mere", &foundVal);
	if (0 != retVal)
	{
		printf("HtGetKeyValue failed!\n");
		goto cleanup;
	}

	if (foundVal != 20)
	{
		printf("Invalid value after get!\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = HtSetKeyValue(usedTable, "john", 10);
	if (0 != retVal)
	{
		printf("HtSetKeyValue failed!\n");
		goto cleanup;
	}

	if (1 != HtHasKey(usedTable, "john"))
	{
		printf("Invalid answer to HtHasKey!\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = HtSetKeyValue(usedTable, "curse", 1);
	if (0 != retVal)
	{
		printf("HtSetKeyValue failed!\n");
		goto cleanup;
	}

	if (1 != HtHasKey(usedTable, "curse"))
	{
		printf("Invalid answer to HtHasKey!\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = HtSetKeyValue(usedTable, "marian", 3);
	if (0 != retVal)
	{
		printf("HtSetKeyValue failed!\n");
		goto cleanup;
	}

	if (1 != HtHasKey(usedTable, "marian"))
	{
		printf("Invalid answer to HtHasKey!\n");
		retVal = -1;
		goto cleanup;
	}

cleanup:
	if (NULL != usedTable)
	{
		if (0 != HtDestroy(&usedTable))
		{
			printf("HtDestroy failed!\n");
			retVal = -1;
		}
	}
	return retVal;
}

int TestStack()
{
	int retVal = -1;
	int foundVal = -1;
	CC_STACK* usedStack = NULL;

	retVal = StCreate(&usedStack);
	if (0 != retVal)
	{
		printf("StCreate failed!\n");
		goto cleanup;
	}

	retVal = StPush(usedStack, 10);
	if (0 != retVal)
	{
		printf("StPush failed!\n");
		goto cleanup;
	}

	if (0 != StIsEmpty(usedStack))
	{
		printf("Invalid answer to StIsEmpty!\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = StPop(usedStack, &foundVal);
	if (0 != retVal)
	{
		printf("StPop failed!\n");
		goto cleanup;
	}

	if (foundVal != 10)
	{
		printf("Invalid value after pop!\n");
		retVal = -1;
		goto cleanup;
	}

	StPush(usedStack, 1);
	StPush(usedStack, 2);
	StPush(usedStack, 3);
	CC_STACK* stackToPush = NULL;
	StCreate(&stackToPush);
	StPush(stackToPush, 1);
	StPush(stackToPush, 4);
	StPush(stackToPush, 5);
	if (0 != StPushStack(usedStack, stackToPush) || usedStack->size != 6)
	{
		printf("StPushStack failed!\n");
		retVal = -1;
		goto cleanup;
	}

	while (usedStack->top != usedStack->base) {
		int value;
		StPop(usedStack, &value);
		printf("%d ", value);
	}

cleanup:
	if (NULL != usedStack)
	{
		if (0 != StDestroy(&usedStack))
		{
			printf("StDestroy failed!\n");
			retVal = -1;
		}
	}
	return retVal;
}

int TestVector()
{
	int retVal = -1;
	int foundVal = 0;
	CC_VECTOR* usedVector = NULL;

	retVal = VecCreate(&usedVector);
	if (0 != retVal)
	{
		printf("VecCreate failed!\n");
		goto cleanup;
	}

	retVal = VecInsertTail(usedVector, 10);  ///Vector: 10
	if (0 != retVal)
	{
		printf("VecInsertTail failed!\n");
		goto cleanup;
	}

	retVal = VecInsertHead(usedVector, 16);  ///Vector: 16 10
	if (0 != retVal)
	{
		printf("VecInsertHead failed!\n");
		goto cleanup;
	}

	if (VecGetCount(usedVector) != 2)
	{
		printf("Invalid count returned!\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = VecInsertAfterIndex(usedVector, 0, 20);  ///Vector: 16 20 10
	if (0 != retVal)
	{
		printf("VecInsertAfterIndex failed!\n");
		goto cleanup;
	}

	retVal = VecRemoveByIndex(usedVector, 0);  ///Vector: 20 10
	if (0 != retVal)
	{
		printf("VecRemoveByIndex failed!\n");
		goto cleanup;
	}

	retVal = VecGetValueByIndex(usedVector, 0, &foundVal);  ///Value: 20
	if (0 != retVal)
	{
		printf("VecGetValueByIndex failed!\n");
		goto cleanup;
	}

	if (foundVal != 20)
	{
		printf("Invalid value found at position 0\n");
		retVal = -1;
		goto cleanup;
	}

	retVal = VecClear(usedVector);
	if (0 != retVal)
	{
		printf("VecClear failed!\n");
		goto cleanup;
	}

	if (0 != VecGetCount(usedVector))
	{
		printf("Invalid count after clear\n");
		retVal = -1;
		goto cleanup;
	}

cleanup:
	if (NULL != usedVector)
	{
		CC_VECTOR* vec1, * vec2;
		VecCreate(&vec1);
		VecCreate(&vec2);
		VecInsertHead(vec1, 16);
		VecInsertHead(vec1, 1);
		VecInsertTail(vec1, 5);

		VecInsertHead(vec2, 6);
		VecInsertHead(vec2, 7);
		VecInsertTail(vec2, 8);

		VecAppend(vec1, vec2);

		VecSort(vec1);
		for (int i = 0; i < vec1->Count; ++i)
		{
			printf("%d ", vec1->Array[i]);
		}
		printf("\n");
		if (0 != VecDestroy(&usedVector))
		{
			printf("VecDestroy failed!\n");
			retVal = -1;
		}
	}
	return retVal;
}