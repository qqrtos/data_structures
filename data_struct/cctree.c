#include "cctree.h"
#include "common.h"
#include <stdio.h>

TREE_NODE* MinNode(TREE_NODE* Root)
{
	///Return the left-most (minimum) node.
	TREE_NODE* LeftNode = Root;

	while (NULL != LeftNode->Left)
	{
		LeftNode = LeftNode->Left;
	}

	return LeftNode;
}

TREE_NODE* MaxNode(TREE_NODE* Root)
{
	///Return the right-most (maximum) node.
	TREE_NODE* RightNode = Root;

	while (NULL != RightNode->Right)
	{
		RightNode = RightNode->Right;
	}

	return RightNode;
}

int height(TREE_NODE* Root)
{
	if (NULL == Root)
	{
		return -1;
	}

	return Root->Height;
}

TREE_NODE* TreeNewNode(int Value)
{
	///Create a new node (as a leaf)

	TREE_NODE* newNode = (TREE_NODE*)malloc(1 * sizeof(TREE_NODE));

	if (NULL == newNode)
	{
		return NULL;
	}

	newNode->Value = Value;
	newNode->Right = NULL;
	newNode->Left = NULL;
	newNode->Height = 0;

	return newNode;
}

int TreeGetBalance(TREE_NODE* Node)
{
	///Return balance value for Node (1,-1,0 means Node is balanced)

	if (NULL == Node)
	{
		return 0;
	}

	return height(Node->Left) - height(Node->Right);
}

int TreeCreate(CC_TREE** Tree)
{
	CC_TREE* newTree = (CC_TREE*)malloc(1 * sizeof(CC_TREE));

	if (NULL == newTree)
	{
		return -1;
	}

	newTree->Root = NULL;
	newTree->Size = 0;

	*Tree = newTree;

	return 0;
}

int TreeDestroy(CC_TREE** Tree)
{
	if (NULL == *Tree)
	{
		return -1;
	}

	CC_TREE* newTree = *Tree;
	free(newTree);

	(*Tree)->Size = 0;
	*Tree = NULL;

	return 0;
}

TREE_NODE* LeftRotate(TREE_NODE* x)
{
	TREE_NODE* y = x->Right;
	TREE_NODE* T2 = y->Left;

	///Perform left rotation 
	y->Left = x;
	x->Right = T2;

	///Update heights 
	x->Height = max(height(x->Left), height(x->Right)) + 1;
	y->Height = max(height(y->Left), height(y->Right)) + 1;

	///Return new root 
	return y;
}

TREE_NODE* RightRotate(TREE_NODE* y)
{
	TREE_NODE* x = y->Left;
	TREE_NODE* T2 = x->Right;

	///Perform right rotation 
	x->Right = y;
	y->Left = T2;

	///Update heights 
	y->Height = max(height(y->Left), height(y->Right)) + 1;
	x->Height = max(height(x->Left), height(x->Right)) + 1;

	///Return new root 
	return x;
}

TREE_NODE* TreeNodeInsertion(TREE_NODE* Node, int Value)
{
	if (Node == NULL)
	{
		return(TreeNewNode(Value));
	}

	if (Value <= Node->Value)
	{
		Node->Left = TreeNodeInsertion(Node->Left, Value);
	}
	else if (Value > Node->Value)
	{
		Node->Right = TreeNodeInsertion(Node->Right, Value);
	}

	Node->Height = 1 + max(height(Node->Left), height(Node->Right));

	///Get the balance factor of this ancestor Node to check whether this Node became unbalanced
	int Balance = TreeGetBalance(Node);

	///If node is unbalanced => 4 cases
	if (Balance > 1 && Value < Node->Left->Value)
	{
		return RightRotate(Node);
	}
	if (Balance < -1 && Value > Node->Right->Value)
	{
		return LeftRotate(Node);
	}
	if (Balance > 1 && Value > Node->Left->Value)
	{
		Node->Left = LeftRotate(Node->Left);
		return RightRotate(Node);
	}
	if (Balance < -1 && Value < Node->Right->Value)
	{
		Node->Right = RightRotate(Node->Right);
		return LeftRotate(Node);
	}

	///return the (unchanged) root pointer
	return Node;
}

int TreeInsert(CC_TREE* Tree, int Value)
{

	if (NULL == Tree)
	{
		return -1;
	}

	///Tree is empty => new Node is the root.
	if (NULL == Tree->Root)
	{
		Tree->Root = TreeNewNode(Value);
		Tree->Size += 1;
		return 0;
	}

	Tree->Root = TreeNodeInsertion(Tree->Root, Value);
	Tree->Size += 1;

	return 0;
}

TREE_NODE* TreeDeleteNode(TREE_NODE* Root, int Value)
{
	if (NULL == Root)
	{
		return Root;
	}

	///Key is smaller => go to left
	if (Value < Root->Value)
	{
		Root->Left = TreeDeleteNode(Root->Left, Value);
	}
	///Key is greater => go to right
	else if (Value > Root->Value)
	{
		Root->Right = TreeDeleteNode(Root->Right, Value);
	}
	else
	{
		///Node with 1 or no children
		if (NULL == Root->Left || NULL == Root->Right)
		{
			TREE_NODE* aux = NULL == Root->Left ? Root->Left : Root->Right;

			///No child case
			if (NULL == aux)
			{
				aux = Root;
				Root = NULL;
			}
			///One child case
			else
			{
				///Copy the contents of the non-empty child 
				*Root = *aux;
			}
			free(aux);
		}
		///Node with 2 children
		else
		{
			///Get the inorder successor (smallest in the right subtree) 
			TREE_NODE* aux = MinNode(Root->Right);

			///Copy the inorder successor's data to this node 
			Root->Value = aux->Value;

			///Delete the inorder successor 
			Root->Right = TreeDeleteNode(Root->Right, aux->Value);
		}
	}

	///If the tree had only one node then return 
	if (Root == NULL)
	{
		return Root;
	}

	Root->Height = 1 + max(height(Root->Left), height(Root->Right));

	///Check balance.
	int Balance = TreeGetBalance(Root);
	
	if (Balance > 1 && TreeGetBalance(Root->Left) >= 0)
	{
		return RightRotate(Root);
	}
	if (Balance > 1 && TreeGetBalance(Root->Left) < 0)
	{
		Root->Left = (Root->Left);
		return RightRotate(Root);
	}
	if (Balance < -1 && TreeGetBalance(Root->Right) <= 0)
	{
		return LeftRotate(Root);
	}
	if (Balance < -1 && TreeGetBalance(Root->Right) > 0)
	{
		Root->Right = RightRotate(Root->Right);
		return LeftRotate(Root);
	}

	return Root;
}

int TreeRemove(CC_TREE* Tree, int Value)
{
	if (NULL == Tree)
	{
		return -1;
	}
	
	///Remove all occurences of Value.
	while (TreeContains(Tree, Value))
	{
		TreeDeleteNode(Tree->Root, Value);
		Tree->Size -= 1;
	}

	return 0;
}

int TreeContainsValue(TREE_NODE* Root, int Value)
{
	///Recursively search for Value.

	if (NULL != Root)
	{
		if (Root->Value == Value)
		{
			return 1;
		}
		else return (TreeContainsValue(Root->Left, Value) || TreeContainsValue(Root->Right, Value));
	}
	else
	{
		return 0;
	}
}

int TreeContains(CC_TREE* Tree, int Value)
{
	if (NULL == Tree)
	{
		return -1;
	}

	return TreeContainsValue(Tree->Root, Value);
}

int TreeGetCount(CC_TREE* Tree)
{
	///Return the size of the tree

	if (NULL == Tree)
	{
		return -1;
	}

	return Tree->Size;
}

int TreeGetHeight(CC_TREE* Tree)
{
	///Return height of the tree. 

	if (NULL == Tree)
	{
		return -1;
	}
	if (NULL == Tree->Root)
	{
		return 0;
	}

	return Tree->Root->Height;
}

void TreeClearNodes(TREE_NODE* Root)
{
	///Free every node.

	if (NULL != Root)
	{
		TreeClearNodes(Root->Left);
		TreeClearNodes(Root->Right);
		free(Root);
	}
}

int TreeClear(CC_TREE* Tree)
{
	///Free nodes and set size to 0

	if (NULL == Tree)
	{
		return -1;
	}

	TreeClearNodes(Tree->Root);
	Tree->Root = NULL;
	Tree->Size = 0;

	return 0;
}

void GetPreorderValue(TREE_NODE* Node, int Index, int* Value)
{
	CC_UNREFERENCED_PARAMETER(Node);
	CC_UNREFERENCED_PARAMETER(Index);
	CC_UNREFERENCED_PARAMETER(Value);
	return;
}

int TreeGetNthPreorder(CC_TREE* Tree, int Index, int* Value)
{

	if (NULL == Tree)
	{
		return -1;
	}
	if (Index > Tree->Size || Index < 0)
	{
		return -1;
	}
	if (NULL == Value)
	{
		return -1;
	}

	GetPreorderValue(Tree->Root, Index, Value);

	return 0;
}

void GetInorderValue(TREE_NODE* Node, int Index, int* Value)
{
	CC_UNREFERENCED_PARAMETER(Node);
	CC_UNREFERENCED_PARAMETER(Index);
	CC_UNREFERENCED_PARAMETER(Value);
	return;
}

int TreeGetNthInorder(CC_TREE* Tree, int Index, int* Value)
{
	if (NULL == Tree)
	{
		return -1;
	}
	if (Index > Tree->Size || Index < 0)
	{
		return -1;
	}
	if (NULL == Value)
	{
		return -1;
	}

	GetInorderValue(Tree->Root, Index, Value);

	return 0;
}

void GetPostorderValue(TREE_NODE* Node, int Index, int* Value)
{
	CC_UNREFERENCED_PARAMETER(Node);
	CC_UNREFERENCED_PARAMETER(Index);
	CC_UNREFERENCED_PARAMETER(Value);
	return;
}

int TreeGetNthPostorder(CC_TREE* Tree, int Index, int* Value)
{
	if (NULL == Tree)
	{
		return -1;
	}
	if (Index > Tree->Size || Index < 0)
	{
		return -1;
	}
	if (NULL == Value)
	{
		return -1;
	}

	GetPostorderValue(Tree->Root, Index, Value);

	return 0;
}

