#include "listNode.cpp"
#include <iostream>

FILE* dbgLogs;

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
	head = allocNode(-1);
	dbgLogs = fopen("list_opt_dbg.txt","w");
}

list_opt::~list_opt()
{
	fclose(dbgLogs);
}

bool list_opt::add(int val)
{
	fprintf(dbgLogs,"\nadd(%d) invoked",val);
	fflush(dbgLogs);

	bool retVal = false;

	while(1)
	{
		//cant fail since head is init in constr
		if(head)
		{
			//head->mtx.lock();

			pNode pred = head;
			pNode curr = NULL;

			if(pred != NULL)
			{
				curr = pred->next;
				if(curr != NULL)
				{	
					//curr->mtx.lock();
					while(curr->val < val)
					{
						if(curr->next != NULL)
						{
							//pred->mtx.unlock();
							pred = curr;
							curr = curr->next;
							//curr->mtx.lock();
						}
						//last element isnt compared yet
						else
						{
							//if element needs to be inserted at the end
							//pred->mtx.unlock();
							pred = curr;
							curr = NULL;

							pred->mtx.lock();
							
							if(validate(pred,curr))
							{
								if(pred->val == val)
								{
									retVal = false;
								}
								else
								{
									pNode newNode = allocNode(val);
									newNode->next = NULL;
									pred->next = newNode;

									retVal = true;
								}
	
								pred->mtx.unlock();
	
								fprintf(dbgLogs,"\nadd(%d) finished",val);
								fflush(dbgLogs);
	
								return retVal;	
							}
							else//the list has been modified; start search from the beginning
							{
								pred->mtx.unlock();
								pred = head;
								curr = pred->next;
							}
						}
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

						fprintf(dbgLogs,"\nadd(%d) finished",val);
						fflush(dbgLogs);

						return retVal;
					}

					pred->mtx.unlock();
					curr->mtx.unlock();

				}
				else//head node is the only node in the list
				{
					head->mtx.lock();

					if(validate(head,NULL))
					{
						if(head->val == val)
						{
							retVal = false;
						}
						else
						{
							if(head->val < val)
							{
								pNode newNode = allocNode(val);
								newNode->next = NULL;
								head->next = newNode;	//LP
								retVal = true;
							}
							else
							{
								pNode newNode = allocNode(val);
								newNode->next = head;
								head = newNode;		//LP
								retVal = true;
							}
						}
					}
					head->mtx.unlock();
				}
			}
		}
	}

	fprintf(dbgLogs,"\nadd(%d) finished",val);
	fflush(dbgLogs);

	return retVal;

}

bool list_opt::remove(int val)
{
	fprintf(dbgLogs,"\nremove(%d) invoked",val);
	fflush(dbgLogs);

	bool retVal = false;

	pNode pred = NULL;
	pNode curr = NULL;

	//cant fail
	if(head)
	{
		head->mtx.lock();

		pred = head;
		curr = head->next;

		if(curr != NULL)
		{
			curr->mtx.lock();
	
			while(curr->val < val)
			{
				if(curr->next != NULL)
				{
					pred->mtx.unlock();
					pred = curr;
					curr = curr->next;
					curr->mtx.lock();
				}
				else
				{
					if(curr->val == val)
					{
						pred->next = curr->next;

						curr->mtx.unlock();
						freeNode(curr);
						curr = NULL;

						retVal = true;	
					}
					else
					{
						curr->mtx.unlock();
						retVal = false;
					}

					pred->mtx.unlock();

					fprintf(dbgLogs,"\nremove(%d) finished",val);
					fflush(dbgLogs);
			
					return retVal;
				}
			}
	
			if(curr->val == val)
			{
				pred->next = curr->next;

				curr->mtx.unlock();
				freeNode(curr);
				curr = NULL;

				retVal = true;
			}
			else
			{
				curr->mtx.unlock();
				retVal = false;
			}
	
			pred->mtx.unlock();
		}
	}

	fprintf(dbgLogs,"\nremove(%d) finished",val);
	fflush(dbgLogs);

	return retVal;

}

bool list_opt::contains(int val)
{
	fprintf(dbgLogs,"\ncontains(%d) invoked",val);
	fflush(dbgLogs);

	bool retVal = false;

	pNode pred = NULL;
	pNode curr = NULL;

	//cant fail
	if(head)
	{
		head->mtx.lock();

		pred = head;
		curr = head->next;

		if(curr != NULL)
		{
			curr->mtx.lock();
	
			while(curr->val < val)
			{
				if(curr->next != NULL)
				{
					pred->mtx.unlock();
					pred = curr;
					curr = curr->next;
					curr->mtx.lock();
				}
				else
				{
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

					fprintf(dbgLogs,"\ncontains(%d) finished",val);
					fflush(dbgLogs);
			
					return retVal;
				}
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
		}
	}

	fprintf(dbgLogs,"\ncontains(%d) finished",val);
	fflush(dbgLogs);

	return retVal;
}

bool list_opt::validate(pNode pred, pNode curr)
{
	pNode node = head;
	while(node->val <= pred->val)
	{
		if(node == pred)
		{
			return (pred->next == curr);
		}
		node = node->next;
	}
	return false;
}

//unprotected
void list_opt::printAll()
{
	FILE* vFile = fopen("validate.txt","w");

	fprintf(dbgLogs,"\nprintAll invoked");
	fflush(dbgLogs);

	pNode curr = head;
	while(curr)
	{
		fprintf(vFile,"\n%d",curr->val);
		fflush(vFile);
		curr = curr->next;
	}
	fprintf(dbgLogs,"\nprintAll finished");
	fflush(dbgLogs);
}
