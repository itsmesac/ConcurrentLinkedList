#include<mutex>

typedef struct _Node
{
	int val;
	_Node* next;
	std::mutex mtx;
}Node,*pNode;



