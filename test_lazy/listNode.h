#include<mutex>

typedef struct _Node
{
	int val;
	_Node* next;
	bool marked;
	std::mutex mtx;
}Node,*pNode;



