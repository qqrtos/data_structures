#include "ccvector.h"
#include "common.h"
#include "string.h"

#define INITIAL_SIZE    100

int VecCreate(CC_VECTOR** Vector)
{
	CC_VECTOR* vec = NULL;

	if (NULL == Vector)
	{
		return -1;
	}

	vec = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
	if (NULL == vec)
	{
		return -1;
	}

	memset(vec, 0, sizeof(*vec));

	vec->Count = 0;
	vec->Size = INITIAL_SIZE;
	vec->Array = (int*)malloc(sizeof(int) * INITIAL_SIZE);
	if (NULL == vec->Array)
	{
		free(vec);
		return -1;
	}

	*Vector = vec;

	return 0;
}

int VecDestroy(CC_VECTOR** Vector)
{
	CC_VECTOR* vec = *Vector;

	if (NULL == Vector)
	{
		return -1;
	}

	free(vec->Array);
	free(vec);

	*Vector = NULL;

	return 0;
}

int VecInsertTail(CC_VECTOR* Vector, int Value)
{
	if (NULL == Vector)
	{
		return -1;
	}

	if (Vector->Count >= Vector->Size)
	{
		/// REALLOC
		int* Array = realloc(Vector->Array, Vector->Count * sizeof(int));
		if (NULL == Array)
		{
			free(Array);
			return -1;
		}
		Vector->Size = Vector->Count + 1;
		Vector->Array = Array;
		return 0;
	}

	Vector->Array[Vector->Count] = Value;
	Vector->Count++;

	return 0;
}

int VecInsertHead(CC_VECTOR* Vector, int Value)
{
	if (NULL == Vector)
	{
		return -1;
	}

	if (Vector->Count >= Vector->Size)
	{
		/// REALLOC
		int* Array = realloc(Vector->Array, Vector->Count * sizeof(int));
		if (NULL == Array)
		{
			free(Array);
			return -1;
		}
		Vector->Size = Vector->Count + 1;
		Vector->Array = Array;
		return 0;
	}

	for (int i = Vector->Count - 1; i >= 0; i--)
	{
		Vector->Array[i + 1] = Vector->Array[i];
	}
	Vector->Array[0] = Value;
	Vector->Count++;

	return 0;
}

int VecInsertAfterIndex(CC_VECTOR* Vector, int Index, int Value)
{
	if (NULL == Vector)
	{
		return -1;
	}

	if (Vector->Count >= Vector->Size)
	{
		/// REALLOC
		int* Array = realloc(Vector->Array, Vector->Count * sizeof(int));
		if (NULL == Array)
		{
			free(Array);
			return -1;
		}
		Vector->Array = Array;
		return 0;
	}

	///For scris de ei.
	/*for (int i = Vector->Count - 1; i >= Index; i--)
	{
		Vector->Array[i + 1] = Vector->Array[i];
	}*/

	for (int i = Vector->Count - 1; i > Index; --i)
	{
		Vector->Array[i + 1] = Vector->Array[i];
	}
	Vector->Array[Index + 1] = Value;
	Vector->Count++;

	return 0;
}

int VecRemoveByIndex(CC_VECTOR* Vector, int Index)
{
	if (NULL == Vector)
	{
		return -1;
	}
	if (Index >= Vector->Count || Index < 0)
	{
		return -1;
	}

	for (int i = Index + 1; i < Vector->Count; ++i)
	{
		Vector->Array[i - 1] = Vector->Array[i];
	}
	--Vector->Count;

	return 0;
}

int VecGetValueByIndex(CC_VECTOR* Vector, int Index, int* Value)
{
	if (NULL == Vector)
	{
		return -1;
	}
	if (Index > Vector->Count || Index < 0)
	{
		return -1;
	}
	*Value = Vector->Array[Index];
	return 0;
}

int VecGetCount(CC_VECTOR* Vector)
{
	if (NULL == Vector)
	{
		return -1;
	}

	return Vector->Count;
}

int VecClear(CC_VECTOR* Vector)
{
	if (NULL == Vector)
	{
		return -1;
	}

	Vector->Count = 0;
	return 0;
}

int VecSort(CC_VECTOR* Vector)
{
	CC_UNREFERENCED_PARAMETER(Vector);
	return -1;
}

int VecAppend(CC_VECTOR* DestVector, CC_VECTOR* SrcVector)
{
	if (NULL == DestVector || NULL == SrcVector)
	{
		return -1;
	}

	for (int i = 0; i < SrcVector->Count; ++i)
	{
		int Value = SrcVector->Array[i];
		VecInsertTail(DestVector, Value);
	}

	return 0;
}
