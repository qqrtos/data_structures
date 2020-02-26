#include "cchashtable.h"
#include "common.h"
#include <memory.h>

///Returns -1 if str1>str2, 1 if str2>str1 and 0 if they're equal.
int _strcmp(char* String1, char* String2)
{
	while (*String1 != '\0' && *String2 != '\0')
	{
		if (*String1 > * String2)
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

	return -1; ///Error finding the next prime.
}

///Returns an index in the hash table from a string of characters. Hash function.
int GetIndexFromKey(CC_HASH_TABLE* HashTable, char* Key)
{
	unsigned long hash = 401;

	while (*Key != '\0')
	{
		hash = ((hash << 4) + (int)(*Key)) % HashTable->Size;
		++Key;
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

	HashTable->Count += 1;

	///Insert at first available spot.
	for (int i = index; i < HashTable->Size; ++i)
	{
		if (1 == HashTable->Array[i]->isAvailable)
		{
			HashTable->Array[i] = entry;
			return 0;
		}
		else if (1 == HashTable->Array[i]->isDeleted)
		{
			free(HashTable->Array[i]);
			HashTable->Array[i] = entry;
			return 0;
		}
	}

	///A spot wasn't found after index, so search for it before.
	for (int i = 0; i < index; ++i)
	{
		if (1 == HashTable->Array[i]->isAvailable)
		{
			HashTable->Array[i] = entry;
			return 0;
		}
		else if (1 == HashTable->Array[i]->isDeleted)
		{
			free(HashTable->Array[i]);
			HashTable->Array[i] = entry;
			return 0;
		}
	}

	///Key couldn't be inserted.
	return -1;
}

int HtRealloc(CC_HASH_TABLE** HashTable)
{
	if (NULL == HashTable)
	{
		return -1;
	}

	if (NULL == (*HashTable)->Array)
	{
		return -1;
	}

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
	newHash->Count = 0;

	///Allocate the new elements.
	for (int i = INITIAL_HASHTABLE_SIZE; i < Size; ++i)
	{
		newHash->Array[i] = (ELEMENT*)malloc(1 * sizeof(ELEMENT));

		if (NULL == newHash->Array[i])
		{
			free(newHash->Array[i]);
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
			int Index = GetIndexFromKey(newHash, entry->Key);
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
		free(newHash);
		return -1;
	}

	newHash->Size = INITIAL_HASHTABLE_SIZE;
	newHash->Count = 0;
	newHash->Array = (ELEMENT**)malloc(INITIAL_HASHTABLE_SIZE * sizeof(ELEMENT*));

	if (NULL == newHash->Array)
	{
		free(newHash->Array);
		return -1;
	}

	///Allocate space for the elements.
	for (int i = 0; i < newHash->Size; ++i)
	{
		newHash->Array[i] = (ELEMENT*)malloc(1 * sizeof(ELEMENT));

		if (NULL == newHash->Array[i])
		{
			free(newHash->Array[i]);
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
	memcpy(entry->Key, Key, 256 * sizeof(char));
	entry->Value = Value;

	int index = GetIndexFromKey(HashTable, Key);

	if (HashTable->Count == HashTable->Size)
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

	return -1; ///Fail
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
		///If spot is available, cluster ended => no key found
		if (1 == HashTable->Array[i]->isAvailable)
		{
			return -1;
		}

		if (0 == HashTable->Array[i]->isDeleted && _strcmp(HashTable->Array[i]->Key, Key) == 0) /// Key was found.
		{
			*Value = HashTable->Array[i]->Value;
			return 0;
		}
	}

	///Search before index.
	for (int i = 0; i < indx; ++i)
	{
		///If spot is available, cluster ended => no key found
		if (1 == HashTable->Array[i]->isAvailable)
		{
			return -1;
		}

		if (0 == HashTable->Array[i]->isDeleted && _strcmp(HashTable->Array[i]->Key, Key) == 0)
		{
			*Value = HashTable->Array[i]->Value;
			return 0;
		}
	}

	return -1; ///Key not found.
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
		if (_strcmp(HashTable->Array[i]->Key, Key) == 0)
		{
			HashTable->Array[i]->isDeleted = 1;
			return 0;
		}
	}

	for (int i = 0; i < Index; ++i)
	{
		if (_strcmp(HashTable->Array[i]->Key, Key) == 0)
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

	int Index = GetIndexFromKey(HashTable, Key);

	for (int i = Index; i < HashTable->Size; ++i)
	{
		if (0 == HashTable->Array[i]->isDeleted && _strcmp(HashTable->Array[i]->Key, Key) == 0)
		{
			return 1;
		}
	}

	for (int i = 0; i < Index; ++i)
	{
		if (0 == HashTable->Array[i]->isDeleted && _strcmp(HashTable->Array[i]->Key, Key) == 0)
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
		free(iterator);
		return -1;
	}

	memset(iterator, 0, sizeof(*iterator));

	iterator->HashTable = HashTable;
	iterator->Index = -1;
	iterator->Current = NULL;

	*Iterator = iterator;

	/// FIND THE FIRST KEY AND SET Key
	for (int i = 0; i < HashTable->Size; ++i)
	{
		if (0 == HashTable->Array[i]->isAvailable && 0 == HashTable->Array[i]->isDeleted)
		{
			(*Iterator)->Current = HashTable->Array[i];
			(*Iterator)->Index = i;
			///strcpy_s(*Key, sizeof(HashTable->Array[i]->Key), HashTable->Array[i]->Key);
			memcpy(Key, HashTable->Array[i]->Key, sizeof(HashTable->Array[i]->Key));
			return 0;
		}
	}

	return -2; ///No keys in the hash table.
}

int HtGetNextKey(CC_HASH_TABLE_ITERATOR* Iterator, char** Key)
{
	if (NULL == Iterator)
	{
		return -1;
	}

	for (int i = Iterator->Index + 1; i < Iterator->HashTable->Size; ++i)
	{
		ELEMENT* entry = Iterator->HashTable->Array[i];
		if (0 == entry->isAvailable && 0 == entry->isDeleted)
		{
			Iterator->Current = entry;
			Iterator->Index = i;
			///strcpy_s(*Key, sizeof(Iterator->HashTable->Array[i]->Key), Iterator->HashTable->Array[i]->Key);
			memcpy(Key, entry->Key, sizeof(entry->Key));
			return 0;
		}
	}

	for (int i = 0; i < Iterator->Index; ++i)
	{
		ELEMENT* entry = Iterator->HashTable->Array[i];
		if (0 == entry->isAvailable && 0 == entry->isDeleted)
		{
			Iterator->Current = entry;
			Iterator->Index = i;
			///strcpy_s(*Key, sizeof(Iterator->HashTable->Array[i]->Key), Iterator->HashTable->Array[i]->Key);
			memcpy(Key, entry->Key, sizeof(entry->Key));
			return 0;
		}
	}

	return -2;
}

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR** Iterator)
{
	if (NULL == *Iterator)
	{
		return -1;
	}

	(*Iterator)->Current = NULL;
	(*Iterator)->HashTable = NULL;
	(*Iterator)->Index = -1;

	free(*Iterator);

	*Iterator = NULL;

	return 0;
}

int HtClear(CC_HASH_TABLE* HashTable)
{
	if (NULL == HashTable)
	{
		return -1;
	}

	///Mark all elements as deleted.
	for (int i = 0; i < HashTable->Size; ++i)
	{
		HashTable->Array[i]->isDeleted = 1;
	}

	return 0;
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
