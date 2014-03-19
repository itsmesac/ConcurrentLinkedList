#include "listNode.cpp"
#include <iostream>

FILE* dbgLogs;

#define MIN -999999
#define MAX 999999

class list_opt
{
public:
	list_opt();
	~list_opt();

	bool add(int val);
	bool remove(int val);
	bool contains(int val);

	void printAll();
private:

	bool validate(pNode pred, pNode curr);
	pNode head;
};

list_opt::list_opt()
{
	head = allocNode(MIN);
	head->node->next = allocNode(MAX);
	//dbgLogs = fopen("list_opt_dbg.txt","w");
}

list_opt::~list_opt()
{
	freeNode(head->node->next);
	freeNode(head);
	//fclose(dbgLogs);
}

bool list_opt::add(int val)
{
	//fprintf(dbgLogs,"\nadd(%d) invoked",val);
	//fflush(dbgLogs);

	bool retVal = false;

	while(1)
	{
		pNode pred = head;
		pNode curr = pred->node->next;
		while(curr->node->val <= val)
		{
			pred = curr;
			curr = curr->node->next;
		}
		pred->mtx.lock();
		curr->mtx.lock();

		if(validate(pred,curr))
		{
			if(curr->node->val == val)
			{
				retVal = false;
			}
			else
			{
				pNode node = allocNode(val);
				node->node->next = curr;
				pred->node->next = node;

				retVal = true;
			}
			
			pred->mtx.unlock();
			curr->mtx.unlock();

	//fprintf(dbgLogs,"\nadd(%d) finished, retVal %d",val,retVal);
	//fflush(dbgLogs);

			return retVal;
		}
		pred->mtx.unlock();
		curr->mtx.unlock();
	}
}

bool list_opt::remove(int val)
{
	bool retVal = false;

	//fprintf(dbgLogs,"\nremove(%d) invoked",val);
	//fflush(dbgLogs);

	while(1)
	{
		pNode pred = head;
		pNode curr = pred->node->next;

		while(curr->node->val < val)
		{
			pred = curr;
			curr = curr->node->next;
		}
		pred->mtx.lock();
		curr->mtx.lock();
		
		if(validate(pred,curr))
		{
			if(curr->node->val == val)
			{
				pred->node->next = curr->node->next;

				pred->mtx.unlock();
				curr->mtx.unlock();
				freeNode(curr);
				curr = NULL;

				retVal = true;
			}
			else
			{
				pred->mtx.unlock();
				curr->mtx.unlock();
				retVal = false;
			}

	//fprintf(dbgLogs,"\nremove(%d) finished, retval %d",val,retVal);
	//fflush(dbgLogs);
			return retVal;
		}

	//fprintf(dbgLogs,"\nremove(%d) failed, retrying",val);
	//fflush(dbgLogs);
		pred->mtx.unlock();
		curr->mtx.unlock();
	}
}

bool list_opt::validate(pNode pred, pNode curr)
{
	
	//fprintf(dbgLogs,"\nvalidate : pred : %u, curr : %u, pred's val : %d, curr's val : %d",pred,curr,pred->val,curr->val);
	//fflush(dbgLogs);

	pNode node = head;
	while(node->node->val <= pred->node->val)
	{
		if(node == pred)
		{
			return (pred->node->next == curr);
		}
		node = node->node->next;
	}
	return false;
}

bool list_opt::contains(int val)
{
	while(1)
	{
		pNode pred = head;
		pNode curr = pred->node->next;

		while(curr->node->val < val)
		{
			pred = curr;
			curr = curr->node->next;
		}

		pred->mtx.lock();
		curr->mtx.lock();

		if(validate(pred,curr))
		{
			bool retVal = (curr->node->val == val);
			pred->mtx.unlock();
			curr->mtx.unlock();
			return retVal;
		}
		pred->mtx.unlock();
		curr->mtx.unlock();
	}
}

//unprotected
void list_opt::printAll()
{
	FILE* vFile = fopen("validate.txt","w");

	//fprintf(dbgLogs,"\nprintAll invoked");
	//fflush(dbgLogs);

	pNode curr = head;
	while(curr)
	{
		fprintf(vFile,"\n%d",curr->node->val);
		fflush(vFile);
		curr = curr->node->next;
	}

	fclose(vFile);
	//fprintf(dbgLogs,"\nprintAll finished");
	//fflush(dbgLogs);
}
