#include<mutex>

template <typename T>
struct _LLNode
{
	int val;
	T* next;
};

typedef struct _Node
{
	struct _LLNode<_Node>* node;
	std::mutex mtx;
}Node,*pNode;
