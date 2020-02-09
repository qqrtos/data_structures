#include "ccheap.h"
#include "common.h"

///Returns -1 if str1>str2, 1 if str2>str1 and 0 if they're equal.
int strcmp(char* String1, char* String2)
{
	while (*String1 != '\0' && *String2 != '\0')
	{
		if (*String1 > *String2)
		{
			return -1;
		}
		else if (*String1 < *String2)
		{
			return 1;
		}
		String1 += 1;
		String2 += 1;
	}
	return 0;
}

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

	if (left < MaxHeap->Count && MaxHeap->Array[left] > MaxHeap->Array[Index])
	{
		///Left node is larger than it's parent.
		largest = left;
	}
	else
	{
		largest = Index;
	}

	if (right < MaxHeap->Count && MaxHeap->Array[right] > MaxHeap->Array[largest])
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
	int smallest = min(left, right);

	if (left < MinHeap->Count && MinHeap->Array[left] < MinHeap->Array[Index])
	{
		smallest = left;
	}
	else
	{
		smallest = Index;
	}

	if (right < MinHeap->Count && MinHeap->Array[right] < MinHeap->Array[smallest])
	{
		smallest = right;
	}

	if (smallest != Index)
	{
		swap(&MinHeap->Array[smallest], &MinHeap->Array[Index]);
		HpCorrectMinHeapError(MinHeap, smallest);
	}
}

///Correct a single error made by inserting an element.
void HpCorrectInsertionError(CC_HEAP* Heap, int Index)
{
	int Parent = HpParent(Index);

	if (Parent >= 0)
	{
		///Swap if the inserted element is larger than the parent (max heap)
		if (strcmp(Heap->Type, "max") == 0 && Heap->Array[Index] > Heap->Array[Parent])
		{
			swap(&Heap->Array[Index], &Heap->Array[Parent]);
			HpCorrectInsertionError(Heap, Parent); ///Recursively go up the heap and check the property
		}
		///Swap if inserted element is lower than parent (min heap)
		else if (strcmp(Heap->Type, "min") == 0 && Heap->Array[Index] < Heap->Array[Parent])
		{
			swap(&Heap->Array[Index], &Heap->Array[Parent]);
			HpCorrectInsertionError(Heap, Parent); ///Recursively go up the heap and check the property
		}
	}
}

int HpCreateMaxHeap(CC_HEAP** MaxHeap, CC_VECTOR* InitialElements)
{
	CC_HEAP* newHeap = (CC_HEAP*)malloc(1 * sizeof(CC_HEAP));
	if (NULL == newHeap)
	{
		return -1; ///Not enough memory.
	}
	char* maxType = "max";
	newHeap->Type = maxType;

	if (NULL != InitialElements)
	{
		///If the optional parameter is not NULL, build max heap from it.

		///Build array from vector elements.
		int* Array = (int*)malloc(InitialElements->Count * sizeof(int));

		if (NULL == Array)
		{
			return -1;
		}

		for (int i = 0; i < InitialElements->Count; ++i)
		{
			Array[i] = InitialElements->Array[i];
		}
		newHeap->Array = Array;
		newHeap->Size = InitialElements->Count;
		newHeap->Count = InitialElements->Count;

		///Vector is larger than current hgeap size.
		if (newHeap->Count > newHeap->Size)
		{
			newHeap->Size = newHeap->Count;
		}

		///Go from (Count / 2 + 1) down to 0 because elements [Count/2+2,...,Count-1] are all leaves that satisfy the requirements.
		for (int i = newHeap->Count / 2 + 1; i >= 0; --i)
		{
			HpCorrectMaxHeapError(newHeap, i);
		}
	}
	else
	{
		newHeap->Array = NULL;
		newHeap->Count = 0;
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
	char* minType = "min";
	newHeap->Type = minType;

	if (NULL != InitialElements && NULL!= InitialElements->Array)
	{
		///Optional parameter is not NULL, so build min heap from it.
		
		int* Array = (int*)malloc(InitialElements->Count * sizeof(int));

		if (NULL == Array)
		{
			return -1;
		}

		for (int i = 0; i < InitialElements->Count; ++i)
		{
			Array[i] = InitialElements->Array[i];
		}
		newHeap->Array = Array;
		newHeap->Count = InitialElements->Count;
		newHeap->Size = InitialElements->Count;

		///Go from (Count / 2 + 1) down to 0 because elements [Count/2+2,...,Count-1] are all leaves that satisfy the requirements.
		for (int i = newHeap->Count / 2 + 1; i >= 0; --i)
		{
			HpCorrectMinHeapError(newHeap, i);
		}
	}
	else
	{
		newHeap->Size = 0;
		newHeap->Array = NULL;
		newHeap->Count = 0;
	}

	*MinHeap = newHeap;

	return 0;
}

int HpDestroy(CC_HEAP** Heap)
{
	if (NULL == *Heap)
	{
		return -1;
	}

	CC_HEAP* newHeap = *Heap;

	if (NULL != newHeap->Array)
	{
		free(newHeap->Array);
	}
	free(newHeap);

	*Heap = NULL;

	return 0;
}

int HpInsert(CC_HEAP* Heap, int Value)
{
	if (NULL == Heap)
	{
		return -1;
	}

	///Empty heap => add the first element as root.
	if (NULL == Heap->Array)
	{
		int* Array = (int*)malloc(INITIAL_HEAP_SIZE * sizeof(int));

		if (NULL == Array)
		{
			return -1;
		}
		Array[0] = Value;

		Heap->Array = Array;
		Heap->Count += 1;
	}
	else ///Heap already contains some elements => insert Value at a valid position
	{
		///Reallocate memory if heap is full.
		if (Heap->Count >= Heap->Size)
		{
			int* Array = (int*)realloc(Heap->Array, (Heap->Size + INITIAL_HEAP_SIZE) * sizeof(int));

			if (NULL == Array)
			{
				return -1;
			}
			Heap->Size += INITIAL_HEAP_SIZE;

			Array[Heap->Count] = Value;
			Heap->Count += 1;

			Heap->Array = Array;
		}
		else
		{
			///Heap not full => insert element and correct errors.
			Heap->Array[Heap->Count] = Value;
			Heap->Count += 1;
		}

		///Correct possible heap error.
		if (strcmp(Heap->Type, "max") == 0)
		{
			HpCorrectInsertionError(Heap, Heap->Count - 1);
		}
		else
		{
			HpCorrectInsertionError(Heap, Heap->Count - 1);
		}
	}

	return 0;
}

int HpRemove(CC_HEAP* Heap, int Value)
{
	if (NULL == Heap)
	{
		return -1;
	}

	///Delete every appearence of Value.
	for (int i = 0; i < Heap->Count; ++i)
	{
		if (Value == Heap->Array[i])
		{
			for (int j = i; j < Heap->Count - 1; ++j)
			{
				Heap->Array[j] = Heap->Array[j + 1];
			}
			Heap->Count -= 1;
			--i;
		}
	}

	///Restore heap propriety.
	if (strcmp(Heap->Type, "max") == 0)
	{
		for (int i = Heap->Count / 2 + 1; i >= 0; --i)
		{
			HpCorrectMaxHeapError(Heap, i);
		}
	}
	else
	{
		for (int i = Heap->Count / 2 + 1; i >= 0; --i)
		{
			HpCorrectMinHeapError(Heap, i);
		}
	}

	return 0;
}

int HpGetExtreme(CC_HEAP* Heap, int* ExtremeValue)
{
	if (NULL == Heap)
	{
		return -1;
	}

	if (NULL == Heap->Array)
	{
		return -1;
	}

	*ExtremeValue = Heap->Array[0];

	return 0;
}

int HpPopExtreme(CC_HEAP* Heap, int* ExtremeValue)
{
	if (NULL == Heap)
	{
		return -1;
	}

	swap(&Heap->Array[0], &Heap->Array[Heap->Count - 1]);
	*ExtremeValue = Heap->Array[Heap->Count - 1];
	Heap->Count -= 1;

	if (strcmp(Heap->Type, "max") == 0)
	{
		HpCorrectMaxHeapError(Heap, 0);
	}
	else
	{
		HpCorrectMinHeapError(Heap, 0);
	}

	return 0;
}

int HpGetElementCount(CC_HEAP* Heap)
{
	if (NULL == Heap)
	{
		return -1;
	}

	return Heap->Count;
}

///DESTRUCTIVE increasing order sorting.
int HpSortToVector(CC_HEAP* Heap, CC_VECTOR* SortedVector)
{
	if (NULL == Heap)
	{
		return -1;
	}

	if (NULL == SortedVector)
	{
		return -1;
	}

	///Prepare sorted vector.
	int* Array = (int*)malloc(Heap->Count * sizeof(int));
	if (NULL == Array)
	{
		return -1;
	}
	SortedVector->Array = Array;
	SortedVector->Count = 0;
	SortedVector->Size = Heap->Count;

	if (strcmp(Heap->Type, "min") == 0)
	{
		///Root is always minimum, so put it at then end of the vector, remove it from heap + correct possible errors.
		while (Heap->Count > 0)
		{
			int Minimum;
			HpPopExtreme(Heap, &Minimum);
			VecInsertTail(SortedVector, Minimum);
		}
	}
	else
	{
		///To sort a max heap in increasing order, we insert the root as a head each time.
		while (Heap->Count > 0)
		{
			int Maximum;
			HpPopExtreme(Heap, &Maximum);
			SortedVector->Count += 1;
			SortedVector->Array[Heap->Count] = Maximum;
		}
	}

	return 0;
}
