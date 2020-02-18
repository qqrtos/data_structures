#include "ccstack.h"
#include "common.h"

int StCreate(CC_STACK** Stack)
{
	CC_STACK* newStack = (CC_STACK*)malloc(1 * sizeof(CC_STACK));
	NODE* node = (NODE*)malloc(1 * sizeof(NODE));   ///Space for a node.

	if (NULL == newStack || NULL == node) {
		free(newStack);
		free(node);
		return -1;  ///If there's no more memory available, return.
	}
	*Stack = newStack;

	node->next = NULL;   ///The first node has no previous and next node.
	node->last = NULL;
	(*Stack)->base = node;
	(*Stack)->top = node;
	(*Stack)->size = 0;

	return 0;
}

int StDestroy(CC_STACK** Stack)
{
	if (NULL == *Stack)
		return -1;
	
	while ((*Stack)->top != (*Stack)->base) {  ///Go from the top to the base and free the memory for each node.
		NODE* node = (*Stack)->top->last;   ///Retain the node previous to the current top node. //????
		free((*Stack)->top);
		(*Stack)->top = node;   ///The new top is the second to last node in the initial stack.
	}
	
	free((*Stack)->top);
	free(*Stack);

	*Stack = NULL;

	return 0;
}

int StPush(CC_STACK* Stack, int Value)
{
	if (NULL == Stack)
		return -1;

	NODE* node = (NODE*)malloc(1 * sizeof(NODE));

	if (NULL == node) {
		free(node);
		return -1;   ///Insufficient memory or smth
	}
	node->value = Value;   ///The value of the new node is assigned.
	node->last = Stack->top;   ///Previous node is the top of the initial stack.
	node->next = NULL;
	Stack->top = node;   ///The new top is the new node.
	Stack->size += 1;

	return 0;
}

int StPop(CC_STACK* Stack, int* Value)
{
	if (0 == Stack->size || NULL == Stack)
		return -1;   ///Empty stack

	NODE* top = Stack->top;   ///Top of the stack
	*Value = top->value;
	Stack->top = top->last;
	Stack->top->next = NULL;   ///The second to last node is now the top of the stack;
	Stack->size -= 1;
	free(top);
	return 0;
}

int StPeek(CC_STACK* Stack, int* Value)
{
	if (0 == Stack->size || NULL == Stack)
		return -1;   ///Empty stack

	*Value = Stack->top->value;
	return 0;
}

int StIsEmpty(CC_STACK* Stack)
{
	if (NULL == Stack)
		return -1;
	return  0 == Stack->size;
}

int StGetCount(CC_STACK* Stack)
{
	if (NULL == Stack)
		return -1;
	return Stack->size;
}

int StClear(CC_STACK* Stack)
{
	if (NULL == Stack)
		return -1;

	if (0 == Stack->size)
		return 0;   ///Empty stack means success.

	while (Stack->top != Stack->base) {
		NODE* node = Stack->top->last; //?????
		free(Stack->top);   ///Free the memory for every node.
		Stack->top = node;
	}
	Stack->size = 0;
	return 0;
}

int StPushStack(CC_STACK* Stack, CC_STACK* StackToPush)
{
	if (NULL == Stack || NULL == StackToPush)
		return -1;
	if (StackToPush->base == StackToPush->top)   ///The case in which the recursion ends.
		return 0;
	else 
	{
		int value;
		StPop(StackToPush, &value);   ///Retain the value from the top of the stack in a variable.
		StPushStack(Stack, StackToPush);
		StPush(Stack, value);   ///Push the values from StackToPush in reverse.
		return 0;
	}
}