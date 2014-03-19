#include "listNode.h"
#include <stdlib.h>

pNode allocNode(int val)
{
	pNode ret = NULL;
	ret = (pNode)malloc(sizeof(Node));
	if(ret)
	{
		ret->node = (_LLNode<Node>*)malloc(sizeof(_LLNode<Node>));
		if(ret->node)
		{
			ret->node->val = val;
			ret->node->next = NULL;
		}
	}
	return ret;
}

void freeNode(pNode node)
{
	if(node)
	{
		if(node->node)
		{
			free(node->node);
		}

		free(node);
	}
}
