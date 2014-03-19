#include"listNode.cpp"

class Window
{

public:
	pNode pred,curr;

	Window(pNode myPred, pNode myCurr)
	{
		pred = myPred;
		curr = myCurr;
	}
};

Window find(pNode head, int val)
{
	pNode pred = NULL, curr = NULL, succ = NULL;
	bool marked = false;
	bool snip;

	retry:

	while(1)
	{
		pred = head;

		if(pred->next)
		{
			curr = (pNode)(pred->next->getRef());
		}
		else
		{
			
		}

		while(1)
		{
			if(curr->next)
			{
				succ = (pNode)(curr->next->get(&marked));
			}
			else
			{
				marked = false;
			}

			while(marked)
			{
				if(pred->next == NULL)
				{
					printf("\nNo entry");
					while(1);
				}

				snip = pred->next->compareAndSet((UINT64)curr,(UINT64)succ,false,false);
				if(!snip)
				{
					goto retry;
				}
				curr = succ;

				if(curr->next)
				{
					succ = (pNode)(curr->next->get(&marked));
				}
				else
				{
					marked = false;
				}
			}

			if((curr->val) >= val)
			{
				//Window* pWindow = new Window(pred,curr);
				Window* pWindow = (Window*)malloc(sizeof(Window));
				pWindow->pred = pred;
				pWindow->curr = curr;

				return *pWindow;
			}
			
			pred = curr;
			curr = succ;
		}
	}
}

class list_nb
{
public:
	list_nb();
	~list_nb();

	bool add(int val);
	bool remove(int val);
	bool contains(int val);

	void printAll();

private:
	pNode head;
};

list_nb::list_nb()
{
	pNode curr = allocNode(MAX);
	head = allocNode(MIN);
	head->next = new AtomicMarkableReference((UINT64)curr,false);
}

list_nb::~list_nb()
{
}

bool list_nb::add(int val)
{
	while(1)
	{
		Window window = find(head,val);
		pNode pred = window.pred;
		pNode curr = window.curr;
		if(curr->val == val)
		{
			return false;
		}
		else
		{
			pNode node = allocNode(val);
			node->next = new AtomicMarkableReference((UINT64)curr,false);
			if(pred->next->compareAndSet((UINT64)curr,(UINT64)node,false,false))
			{
				return true;
			}
		}
	}
}

bool list_nb::remove(int val)
{
	bool temp = false;
	bool snip = false;
	while(1)
	{
		Window window = find(head,val);
		pNode pred = window.pred;
		pNode curr = window.curr;
		if(curr->val != val)
		{
			return false;
		}
		else
		{
			pNode succ = NULL;

			if(curr->next)
			{
				succ = (pNode)(curr->next->getRef());
				snip = curr->next->attemptMark((UINT64)succ,true);
			}
			else
			{
				snip = false;
			}
			
			if(!snip)
			{
				continue;
			}

			if(pred->next)
			{
				pred->next->compareAndSet((UINT64)curr,(UINT64)succ,false,false);
				return true;
			}
			else
			{
				printf("\nNo entry #2");
				while(1);
			}
		}
	}
}

bool list_nb::contains(int val)
{
	bool marked = false;
	bool temp = false;
	pNode curr = head;
	while(curr->val < val)
	{
		pNode succ =  NULL;
		if(curr->next)
		{
			curr = (pNode)(curr->next->getRef());
			if(curr->next)
			{
				succ = (pNode)(curr->next->get(&marked));
			}
			else
			{
				marked = false;
			}
		}
	}
	return ((curr->val == val) && (!marked));
}


void list_nb::printAll()
{
	FILE* vFile = fopen("validate.txt","w");
	//FILE* vFile = stdout;
	bool temp = false;
	//fprintf(dbgLogs,"\nprintAll invoked");
	//fflush(dbgLogs);

	pNode curr = head;
	while(curr->val < MAX)
	{
		fprintf(vFile,"\n%d",curr->val);
		fflush(vFile);

		if(curr->next)
		{
			curr = (pNode)(curr->next->getRef());
		}
	}

	fclose(vFile);
	//fprintf(dbgLogs,"\nprintAll finished");
	//fflush(dbgLogs);
}
