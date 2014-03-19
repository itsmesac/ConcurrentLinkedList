#include "listNode.cpp"
#include <iostream>

FILE* dbgLogs;

#define MIN -999999
#define MAX 999999

class list_cgl
{
public:
	list_cgl();
	~list_cgl();

	bool add(int val);
	bool remove(int val);
	bool contains(int val);

	void printAll();
private:

	bool validate(pNode pred, pNode curr);
	pNode head;
	std::mutex mtx;

};

list_cgl::list_cgl()
{
	head = allocNode(MIN);
	head->next = allocNode(MAX);
	//dbgLogs = fopen("list_cgl_dbg.txt","w");
}

list_cgl::~list_cgl()
{
	freeNode(head->next);
	freeNode(head);
	//fclose(dbgLogs);
}

bool list_cgl::add(int val)
{
	//fprintf(dbgLogs,"\nadd(%d) invoked",val);
	//fflush(dbgLogs);

	bool retVal = false;

	mtx.lock();

	pNode pred = head;
	pNode curr = pred->next;
	while(curr->val < val)
	{
		pred = curr;
		curr = curr->next;
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

	mtx.unlock();

	return retVal;
}

bool list_cgl::remove(int val)
{
	bool retVal = false;

	mtx.lock();

	pNode pred = head;
	pNode curr = pred->next;

	while(curr->val < val)
	{
		pred = curr;
		curr = curr->next;
	}
	
	if(curr->val == val)
	{
		pred->next = curr->next;
		freeNode(curr);
		curr = NULL;
		retVal = true;
	}
	else
	{
		retVal = false;
	}
	mtx.unlock();
	return retVal;
}

bool list_cgl::contains(int val)
{
	bool retVal = false;

	mtx.lock();

	pNode pred = head;
	pNode curr = pred->next;

	while(curr->val < val)
	{
		pred = curr;
		curr = curr->next;
	}
	retVal = (curr->val == val);

	mtx.unlock();

	return retVal;
}

//unprotected
void list_cgl::printAll()
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
