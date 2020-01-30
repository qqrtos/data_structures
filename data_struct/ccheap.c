#include "ccheap.h"
#include "common.h"

///Swaps two integers
void swap(int* x, int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

///Return the index for the parent of a node.
int HpParent(int childIndex)
{
	return (childIndex - 1) / 2;
}

///Return the index of the node to the left.
int HpLeft(int Index)
{
	return 2 * Index + 1;
}

///Return the index of the node to the right.
int HpRight(int Index)
{
	return 2 * Index + 2;
}

///Corrects a single error (misplaced node / node is larger than the parent) in a max heap.
void HpCorrectMaxHeapError(CC_HEAP* MaxHeap, int Index)
{
	int left = HpLeft(Index);
	int right = HpRight(Index);
	int largest = max(left, right);

	if (left < MaxHeap->Size && MaxHeap->Array[left] > MaxHeap->Array[Index])
	{
		///Left node is larger than it's parent.
		largest = left;
	}
	else
	{
		largest = Index;
	}

	if (right < MaxHeap->Size && MaxHeap->Array[right] > MaxHeap->Array[largest])
	{
		///Right node is larger than it's parent
		largest = right;
	}

	if (largest != Index)
	{
		///Swap the largest node with it's parent recursively, until it is in it's right place.
		swap(&MaxHeap->Array[largest], &MaxHeap->Array[Index]);
		HpCorrectMaxHeapError(MaxHeap, largest);
	}
}

///Similar to the same function for MaxHeap.
void HpCorrectMinHeapError(CC_HEAP* MinHeap, int Index)
{
	int left = HpLeft(Index);
	int right = HpRight(Index);
	int smallest = max(left, right);

	if (left < MinHeap->Size && MinHeap->Array[left] < MinHeap->Array[Index])
	{
		smallest = left;
	}
	else
	{
		smallest = Index;
	}

	if (right < MinHeap->Size && MinHeap->Array[right] < MinHeap->Array[smallest])
	{
		smallest = right;
	}

	if (smallest != Index)
	{
		swap(&MinHeap->Array[smallest], &MinHeap->Array[Index]);
		HpCorrectMaxHeapError(MinHeap, smallest);
	}
}

int HpCreateMaxHeap(CC_HEAP** MaxHeap, CC_VECTOR* InitialElements)
{
	CC_HEAP* newHeap = (CC_HEAP*)malloc(1 * sizeof(CC_HEAP));
	if (NULL == newHeap)
	{
		return -1; ///Not enough memory.
	}

	if (NULL != InitialElements)
	{
		///If the optional parameter is not NULL, build max heap from it.

		newHeap->Array = InitialElements->Array;
		newHeap->Size = InitialElements->Count;

		///Go from (size / 2 + 1) down to 0 because elements [size/2+2,...,size-1] are all leaves that satisfy the requirements.
		for (int i = newHeap->Size / 2 + 1; i >= 0; --i)
		{
			HpCorrectMaxHeapError(newHeap, i);
		}
	}
	else
	{
		newHeap->Array = NULL;
		newHeap->Size = 0;
	}

	*MaxHeap = newHeap;

	return 0;
}

int HpCreateMinHeap(CC_HEAP** MinHeap, CC_VECTOR* InitialElements)
{
	CC_HEAP* newHeap = (CC_HEAP*)malloc(1 * sizeof(CC_HEAP));
	if (NULL == newHeap)
	{
		return -1;
	}

	if (NULL != InitialElements)
	{
		///Optional parameter is not NULL, so build min heap from it.

		newHeap->Array = InitialElements->Array;
		newHeap->Size = InitialElements->Count;

		///Go from (size / 2 + 1) down to 0 because elements [size/2+2,...,size-1] are all leaves that satisfy the requirements.
		for (int i = newHeap->Size / 2 + 1; i >= 0; --i)
		{
			HpCorrectMinHeapError(newHeap, i);
		}
	}
	else
	{
		newHeap->Array = NULL;
		newHeap->Size = 0;
	}

	*MinHeap = newHeap;

	return 0;
}

int HpDestroy(CC_HEAP** Heap)
{
	CC_UNREFERENCED_PARAMETER(Heap);
	return -1;
}

int HpInsert(CC_HEAP* Heap, int Value)
{
	CC_UNREFERENCED_PARAMETER(Heap);
	CC_UNREFERENCED_PARAMETER(Value);
	return -1;
}

int HpRemove(CC_HEAP* Heap, int Value)
{
	CC_UNREFERENCED_PARAMETER(Heap);
	CC_UNREFERENCED_PARAMETER(Value);
	return -1;
}

int HpGetExtreme(CC_HEAP* Heap, int* ExtremeValue)
{
	CC_UNREFERENCED_PARAMETER(Heap);
	CC_UNREFERENCED_PARAMETER(ExtremeValue);
	return -1;
}

int HpPopExtreme(CC_HEAP* Heap, int* ExtremeValue)
{
	CC_UNREFERENCED_PARAMETER(Heap);
	CC_UNREFERENCED_PARAMETER(ExtremeValue);
	return -1;
}

int HpGetElementCount(CC_HEAP* Heap)
{
	CC_UNREFERENCED_PARAMETER(Heap);
	return -1;
}

int HpSortToVector(CC_HEAP* Heap, CC_VECTOR* SortedVector)
{
	CC_UNREFERENCED_PARAMETER(Heap);
	CC_UNREFERENCED_PARAMETER(SortedVector);
	return -1;
}
