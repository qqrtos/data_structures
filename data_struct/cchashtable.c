#include "cchashtable.h"
#include "common.h"
#include <memory.h>
#include <string.h>

int isPrime(int Value)
{
	/// 0 -> Value is not prime
	/// 1 -> Value is prime
	if (Value < 2 || (Value > 2 && Value % 2 == 0))
	{
		return 0;
	}

	for (unsigned int d = 3; d <= Value / d; d += 2)
	{
		if (Value % d == 0)
		{
			return 0;
		}
	}

	return 1; ///Value is prime
}

///Get the next prime after the old size of HashTable.
int NextPrime(CC_HASH_TABLE* HashTable)
{
	for (int i = HashTable->Size * 2 + 1; i < INT_MAX; ++i)
	{
		if (isPrime(i))
		{
			return i;
		}
	}
	return -1;
}

///Returns an index in the hash table from a string of characters. Hash function.
int GetIndexFromKey(CC_HASH_TABLE* HashTable, char* Key)
{
	unsigned long hash = 401;

	for (unsigned int i = 0; i < strlen(Key); ++i)
	{
		hash = ((hash << 4) + (int)(Key[i])) % HashTable->Size;
	}
	return hash % HashTable->Size;
}

int HtInsert(CC_HASH_TABLE* HashTable, int index, ELEMENT* entry)
{
	///Key already in hash table.
	int Val;
	if (HtGetKeyValue(HashTable, entry->Key, &Val) == 0)
	{
		return -1;
	}

	///Insert at first available spot.
	for (int i = index; i < HashTable->Size; ++i)
	{
		if (1 == HashTable->Array[index]->isAvailable)
		{
			HashTable->Array[index] = entry;
			return 0;
		}
		else if (1 == HashTable->Array[index]->isDeleted)
		{
			free(HashTable->Array[index]);
			HashTable->Array[index] = entry;
			return 0;
		}
	}

	///A spot wasn't found after index, so search for it before.
	for (int i = 0; i < index; ++i)
	{
		if (1 == HashTable->Array[index]->isAvailable)
		{
			HashTable->Array[index] = entry;
			return 0;
		}
		else if (1 == HashTable->Array[index]->isDeleted)
		{
			free(HashTable->Array[index]);
			HashTable->Array[index] = entry;
			return 0;
		}
	}

	///Key couldn't be inserted.
	return -1;
}

int HtRealloc(CC_HASH_TABLE** HashTable)
{
	///Hash table is full, so resize it.
	CC_HASH_TABLE* newHash;
	HtCreate(&newHash);

	int Size = NextPrime(*HashTable);

	ELEMENT** Array = (ELEMENT**)realloc(newHash->Array, Size * sizeof(ELEMENT));

	if (NULL == Array)
	{
		return -1;
	}
	newHash->Size = Size;
	newHash->Array = Array;

	///Allocate the new elements.
	for (int i = INITIAL_SIZE; i < Size; ++i)
	{
		newHash->Array[i] = (ELEMENT*)malloc(1 * sizeof(ELEMENT));

		if (NULL == newHash->Array[i])
		{
			return -1;
		}

		newHash->Array[i]->isAvailable = 1;
		newHash->Array[i]->isDeleted = 0;
	}

	///Copy the elements in the inital hash table into the newer, bigger one.
	for (int i = 0; i < (*HashTable)->Size; ++i)
	{
		ELEMENT* entry = (*HashTable)->Array[i];

		///Transfer old keys to the new hash table.
		if (0 == entry->isAvailable && 0 == entry->isDeleted)
		{
			int Index = GetIndexFromKey(newHash, (*HashTable)->Array[i]->Key);
			HtInsert(newHash, Index, entry);
		}
		else
		{
			///Free unnecessary elements (deleted/unnalocated).
			free((*HashTable)->Array[i]);
		}
	}

	(*HashTable)->Size = newHash->Size;
	(*HashTable)->Array = newHash->Array;
	*HashTable = newHash;

	return 0; ///Success
}

int HtCreate(CC_HASH_TABLE** HashTable)
{
	///Create a new hash table.
	CC_HASH_TABLE* newHash = (CC_HASH_TABLE*)malloc(1 * sizeof(CC_HASH_TABLE));

	if (NULL == newHash)
	{
		return -1;
	}

	newHash->Size = INITIAL_SIZE;
	newHash->Array = (ELEMENT**)malloc(INITIAL_SIZE * sizeof(ELEMENT*));

	if (NULL == newHash->Array)
	{
		return -1;
	}

	///Allocate space for the elements.
	for (int i = 0; i < newHash->Size; ++i)
	{
		newHash->Array[i] = (ELEMENT*)malloc(1 * sizeof(ELEMENT));

		if (NULL == newHash->Array[i])
		{
			return -1;
		}

		newHash->Array[i]->isDeleted = 0;
		newHash->Array[i]->isAvailable = 1; ///All spots are initially available.
	}

	*HashTable = newHash;

	return 0;
}

int HtDestroy(CC_HASH_TABLE** HashTable)
{
	///Destroy hash table.
	if (NULL == *HashTable)
	{
		return -1;
	}

	CC_HASH_TABLE* newHash = (*HashTable);

	for (int i = newHash->Size - 1; i >= 0; --i)
	{
		ELEMENT* current = newHash->Array[i];
		free(current);
	}

	free(newHash->Array);
	free(newHash);

	*HashTable = NULL;

	return 0;
}

int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value)
{
	///Set HashTable[Key]
	if (NULL == HashTable)
	{
		return -1;
	}
	if (NULL == Key)
	{
		return -1;
	}

	ELEMENT* entry = (ELEMENT*)malloc(1 * sizeof(ELEMENT));

	if (NULL == entry)
	{
		return -1;
	}
	entry->isDeleted = 0;
	entry->isAvailable = 0;
	strcpy_s(entry->Key, 256 * sizeof(char), Key);
	entry->Value = Value;

	int index = GetIndexFromKey(HashTable, Key);

	if (HtGetKeyCount(HashTable) == HashTable->Size)
	{
		///Table is too full, so resize it and repeat the search.
		HtRealloc(&HashTable);
		if (HtInsert(HashTable, index, entry) != -1)
			return 0;
	}
	else
	{
		///There are enough spaces, so insert key.
		if (HtInsert(HashTable, index, entry) != -1)
			return 0;
	}

	return -1;
}

int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int* Value)
{
	///Return the value for a given Key.
	if (NULL == HashTable)
	{
		return -1;
	}

	if (NULL == Key)
	{
		return -1;
	}

	int indx = GetIndexFromKey(HashTable, Key); ///Compute the index using the hash function.

	///Search after index.
	for (int i = indx; i < HashTable->Size; ++i)
	{
		if (0 == HashTable->Array[i]->isDeleted && strcmp(HashTable->Array[i]->Key, Key) == 0) /// Key was found.
		{
			*Value = HashTable->Array[i]->Value;
			return 0;
		}
	}

	///Search before index.
	for (int i = 0; i < indx; ++i)
	{
		if (0 == HashTable->Array[i]->isDeleted && strcmp(HashTable->Array[i]->Key, Key) == 0)
		{
			*Value = HashTable->Array[i]->Value;
			return 0;
		}
	}

	return -1;
}

int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key)
{
	if (NULL == HashTable)
	{
		return -1;
	}

	if (NULL == Key)
	{
		return -1;
	}

	int Index = GetIndexFromKey(HashTable, Key);

	for (int i = Index; i < HashTable->Size; ++i)
	{
		if (strcmp(HashTable->Array[i]->Key, Key) == 0)
		{
			HashTable->Array[i]->isDeleted = 1;
			return 0;
		}
	}

	for (int i = 0; i < Index; ++i)
	{
		if (strcmp(HashTable->Array[i]->Key, Key) == 0)
		{
			HashTable->Array[i]->isDeleted = 1;
			return 0;
		}
	}

	return -1;
}

int HtHasKey(CC_HASH_TABLE* HashTable, char* Key)
{
	///Check whether
	if (NULL == HashTable)
	{
		return -1;
	}

	if (NULL == Key)
	{
		return -1;
	}

	for (int i = 0; i < HashTable->Size; ++i)
	{
		if (0 == HashTable->Array[i]->isDeleted && strcmp(HashTable->Array[i]->Key, Key) == 0)
		{
			return 1;
		}
	}

	return 0;
}

int HtGetFirstKey(CC_HASH_TABLE* HashTable, CC_HASH_TABLE_ITERATOR** Iterator, char** Key)
{
	CC_HASH_TABLE_ITERATOR* iterator = NULL;

	if (NULL == HashTable)
	{
		return -1;
	}
	if (NULL == Iterator)
	{
		return -1;
	}
	if (NULL == Key)
	{
		return -1;
	}

	iterator = (CC_HASH_TABLE_ITERATOR*)malloc(sizeof(CC_HASH_TABLE_ITERATOR));
	if (NULL == iterator)
	{
		return -1;
	}

	memset(iterator, 0, sizeof(*iterator));

	iterator->HashTable = HashTable;
	iterator->Index = -1;

	*Iterator = iterator;

	// FIND THE FIRST KEY AND SET Key
	for (int i = 0; i < HashTable->Size; ++i)
	{
		if (1 == HashTable->Array[i]->isAvailable && 0 == HashTable->Array[i]->isDeleted)
		{
			(*Iterator)->Index = i;
			strcpy_s((*Iterator)->Key, sizeof(HashTable->Array[i]->Key), HashTable->Array[i]->Key);
			strcpy_s(*Key, sizeof(HashTable->Array[i]->Key), HashTable->Array[i]->Key);
			return 0;
		}
	}

	return -2; ///No keys in the hash table.
}

int HtGetNextKey(CC_HASH_TABLE_ITERATOR* Iterator, char** Key)
{
	CC_UNREFERENCED_PARAMETER(Key);
	CC_UNREFERENCED_PARAMETER(Iterator);
	return -1;
}

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR** Iterator)
{
	CC_UNREFERENCED_PARAMETER(Iterator);
	return -1;
}

int HtClear(CC_HASH_TABLE* HashTable)
{
	CC_UNREFERENCED_PARAMETER(HashTable);
	return -1;
}

int HtGetKeyCount(CC_HASH_TABLE* HashTable)
{
	if (NULL == HashTable)
	{
		return -1;
	}

	int count = 0;
	for (int i = 0; i < HashTable->Size; ++i)
	{
		if (0 == HashTable->Array[i]->isAvailable && 0 == HashTable->Array[i]->isDeleted)
		{
			++count;
		}
	}

	return count;
}
