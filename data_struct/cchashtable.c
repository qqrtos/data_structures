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
}

///Returns an index in the hash table from a string of characters.
int GetIndexFromKey(CC_HASH_TABLE* HashTable, char* Key)
{
	int hash = 401;

	for (unsigned int i = 0; i < strlen(Key); ++i)
	{
		hash = ((hash << 4) + (int)(Key[i])) % HashTable->Size;
	}
	return hash % HashTable->Size;
}

int HtCreate(CC_HASH_TABLE** HashTable)
{
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

	for (int i = 0; i < newHash->Size; ++i)
	{
		newHash->Array[i] = (ELEMENT*)malloc(1 * sizeof(ELEMENT));
		if (NULL == newHash->Array[i])
		{
			return -1;
		}
	}

	*HashTable = newHash;

	return 0;
}

int HtDestroy(CC_HASH_TABLE** HashTable)
{
	if (NULL == *HashTable)
	{
		return -1;
	}

	CC_HASH_TABLE* newHash = (*HashTable);

	for (int i = newHash->Size - 1; i >= 0; --i)
	{
		free(newHash->Array[i]);
	}

	free(newHash->Array);
	free(newHash);

	*HashTable = NULL;

	return 0;
}

int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value)
{
	CC_UNREFERENCED_PARAMETER(HashTable);
	CC_UNREFERENCED_PARAMETER(Key);
	CC_UNREFERENCED_PARAMETER(Value);
	return -1;
}

int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int* Value)
{
	CC_UNREFERENCED_PARAMETER(HashTable);
	CC_UNREFERENCED_PARAMETER(Key);
	CC_UNREFERENCED_PARAMETER(Value);
	return -1;
}

int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key)
{
	CC_UNREFERENCED_PARAMETER(HashTable);
	CC_UNREFERENCED_PARAMETER(Key);
	return -1;
}

int HtHasKey(CC_HASH_TABLE* HashTable, char* Key)
{
	CC_UNREFERENCED_PARAMETER(HashTable);
	CC_UNREFERENCED_PARAMETER(Key);
	return -1;
}

int HtGetFirstKey(CC_HASH_TABLE* HashTable, CC_HASH_TABLE_ITERATOR** Iterator, char** Key)
{
	CC_HASH_TABLE_ITERATOR* iterator = NULL;

	CC_UNREFERENCED_PARAMETER(Key);

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
	// INITIALIZE THE REST OF THE FIELDS OF iterator

	*Iterator = iterator;

	// FIND THE FIRST KEY AND SET Key

	return 0;
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
	CC_UNREFERENCED_PARAMETER(HashTable);
	return -1;
}
