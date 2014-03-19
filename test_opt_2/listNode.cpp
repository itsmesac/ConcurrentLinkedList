#include "listNode.h"
#include <stdlib.h>

pNode allocNode(int val)
{
	pNode ret = NULL;
	ret = (pNode)malloc(sizeof(Node));
	if(ret)
	{
		ret->val = val;
		ret->next = NULL;
	}

	return ret;
}

void freeNode(pNode node)
{
	if(node)
	{
		free(node);
	}
}
