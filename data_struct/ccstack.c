#include "ccstack.h"
#include "common.h"

int StCreate(CC_STACK** Stack)
{
	/*CC_UNREFERENCED_PARAMETER(Stack);*/

	*Stack = (CC_STACK*)malloc(1 * sizeof(CC_STACK));  ///Allocate space for the stack.
	NODE* node = (NODE*)malloc(1 * sizeof(NODE));   ///Space for a node.
	node->next = NULL;
	node->prev = NULL;   ///The first node has no previous node and no next node.
	(*Stack)->base = node;
	(*Stack)->top = node;
	(*Stack)->count = 0;
	return 0;
}

int StDestroy(CC_STACK** Stack)
{
	/*CC_UNREFERENCED_PARAMETER(Stack);*/

	while ((*Stack)->top != (*Stack)->base) {  ///Go from the top to the base and free the memory for each node.
		NODE* node = (*Stack)->top->prev;   ///Retain the node previous to the current top node.
		free((*Stack)->top);
		(*Stack)->top = node;   ///The new top is the second to last node in the initial stack.
	}
	free(*Stack);
	return 0;
}

int StPush(CC_STACK* Stack, int Value)
{
	/*CC_UNREFERENCED_PARAMETER(Stack);
	CC_UNREFERENCED_PARAMETER(Value);*/

	NODE* node = (NODE*)malloc(1 * sizeof(NODE));
	node->value = Value;   ///The vvalue of the new node is assigned.
	node->next = NULL;   ///The new node is the current top, so the next node is NULL.
	node->prev = Stack->top;   ///Previous node is the top of the initial stack.
	Stack->top = node;   ///The new top is the new node.
	return 0;
}

int StPop(CC_STACK* Stack, int* Value)
{
	CC_UNREFERENCED_PARAMETER(Stack);
	CC_UNREFERENCED_PARAMETER(Value);
	return -1;
}

int StPeek(CC_STACK* Stack, int* Value)
{
	CC_UNREFERENCED_PARAMETER(Stack);
	CC_UNREFERENCED_PARAMETER(Value);
	return -1;
}

int StIsEmpty(CC_STACK* Stack)
{
	CC_UNREFERENCED_PARAMETER(Stack);
	return -1;
}

int StGetCount(CC_STACK* Stack)
{
	CC_UNREFERENCED_PARAMETER(Stack);
	return -1;
}

int StClear(CC_STACK* Stack)
{
	CC_UNREFERENCED_PARAMETER(Stack);
	return -1;
}

int StPushStack(CC_STACK* Stack, CC_STACK* StackToPush)
{
	CC_UNREFERENCED_PARAMETER(Stack);
	CC_UNREFERENCED_PARAMETER(StackToPush);
	return -1;
}