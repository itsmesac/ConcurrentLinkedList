#include "listNode.cpp"
#include <iostream>

FILE* dbgLogs;

#define MIN -999999
#define MAX 999999

class list_lazy
{
public:
	list_lazy();
	~list_lazy();

	bool add(int val);
	bool remove(int val);
	bool contains(int val);

	void printAll();
private:

	bool validate(pNode pred, pNode curr);
	pNode head;
};

list_lazy::list_lazy()
{
	head = allocNode(MIN);
	head->next = allocNode(MAX);
	//dbgLogs = fopen("list_lazy_dbg.txt","w");
}

list_lazy::~list_lazy()
{
	freeNode(head->next);
	freeNode(head);
	//fclose(dbgLogs);
}

bool list_lazy::add(int val)
{
	//fprintf(dbgLogs,"\nadd(%d) invoked",val);
	//fflush(dbgLogs);

	bool retVal = false;

	while(1)
	{
		pNode pred = head;
		pNode curr = pred->next;
		while(curr->val <= val)
		{
			pred = curr;
			curr = curr->next;
		}
		pred->mtx.lock();
		curr->mtx.lock();

		if(validate(pred,curr))
		{
			if(curr->val == val)
			{
				retVal = false;
			}
			else
			{
				pNode node = allocNode(val);
				node->next = curr;
				pred->next = node;

				retVal = true;
			}
			pred->mtx.unlock();
			curr->mtx.unlock();

			return retVal;
		}
		pred->mtx.unlock();
		curr->mtx.unlock();
	}
}

bool list_lazy::remove(int val)
{
	bool retVal = false;

	while(1)
	{
		pNode pred = head;
		pNode curr = pred->next;

		while(curr->val < val)
		{
			pred = curr;
			curr = curr->next;
		}
		pred->mtx.lock();
		curr->mtx.lock();
		
		if(validate(pred,curr))
		{
			if(curr->val != val)
			{
				curr->mtx.unlock();
				retVal = false;
			}
			else
			{
				curr->marked = true;
				pred->next = curr->next;

				curr->mtx.unlock();
				//freeNode(curr);
				//curr = NULL;

				retVal = true;
			}

			pred->mtx.unlock();
			return retVal;
		}
		pred->mtx.unlock();
		curr->mtx.unlock();
	}
}

bool list_lazy::validate(pNode pred, pNode curr)
{
	return ((!pred->marked) && (!curr->marked) && (pred->next == curr));
}

bool list_lazy::contains(int val)
{
	pNode curr = head;

	while(curr->val < val)
	{
		curr = curr->next;
	}
	return ((curr->val == val) && (!curr->marked));
}

//unprotected
void list_lazy::printAll()
{
	FILE* vFile = fopen("validate.txt","w");

	//fprintf(dbgLogs,"\nprintAll invoked");
	//fflush(dbgLogs);

	pNode curr = head;
	while(curr)
	{
		fprintf(vFile,"\n%d",curr->val);
		fflush(vFile);
		curr = curr->next;
	}
	fclose(vFile);
	//fprintf(dbgLogs,"\nprintAll finished");
	//fflush(dbgLogs);
}
