#include "listNode.cpp"
#include <iostream>

FILE* dbgLogs;

#define MIN -999999
#define MAX 999999

class list_fgl
{
public:
	list_fgl();
	~list_fgl();

	bool add(int val);
	bool remove(int val);
	bool contains(int val);

	void printAll();
private:

	bool validate(pNode pred, pNode curr);
	pNode head;
};

list_fgl::list_fgl()
{
	head = allocNode(MIN);
	head->next = allocNode(MAX);
	//dbgLogs = fopen("list_fgl_dbg.txt","w");
}

list_fgl::~list_fgl()
{
	freeNode(head->next);
	freeNode(head);
	//fclose(dbgLogs);
}

bool list_fgl::add(int val)
{
	//fprintf(dbgLogs,"\nadd(%d) invoked",val);
	//fflush(dbgLogs);

	bool retVal = false;

	head->mtx.lock();

	pNode pred = head;
	pNode curr = pred->next;

	curr->mtx.lock();

	while(curr->val < val)
	{
		pred->mtx.unlock();
		pred = curr;
		curr = curr->next;
		curr->mtx.lock();
	}

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

	curr->mtx.unlock();
	pred->mtx.unlock();

	return retVal;
}

bool list_fgl::remove(int val)
{
	bool retVal = false;

	pNode pred = NULL;
	pNode curr = NULL;

	head->mtx.lock();
	pred = head;
	curr = pred->next;
	curr->mtx.lock();

	while(curr->val < val)
	{
		pred->mtx.unlock();
		pred = curr;
		curr = curr->next;
		curr->mtx.lock();
	}
		
	if(curr->val == val)
	{
		pred->next = curr->next;
		curr->mtx.unlock();
		//freeNode(curr);
		//curr = NULL;
		retVal = true;
	}
	else
	{
		curr->mtx.unlock();
		retVal = false;
	}
	pred->mtx.unlock();
	return retVal;
}

bool list_fgl::contains(int val)
{
	bool retVal = false;

	pNode pred = NULL;
	pNode curr = NULL;

	head->mtx.lock();
	pred = head;
	curr = pred->next;
	curr->mtx.lock();

	while(curr->val < val)
	{
		pred->mtx.unlock();
		pred = curr;
		curr = curr->next;
		curr->mtx.lock();
	}
		
	if(curr->val == val)
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}

	curr->mtx.unlock();
	pred->mtx.unlock();
	return retVal;
}

//unprotected
void list_fgl::printAll()
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
