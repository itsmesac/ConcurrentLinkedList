#include "amr.cpp"

#define MIN -999999
#define MAX 999999

typedef struct _Node
{
	int val;
	AtomicMarkableReference* next;

}Node,*pNode;



