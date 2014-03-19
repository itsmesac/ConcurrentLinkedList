#include<iostream>
#include<thread>
#include "list_opt.cpp"
#include<time.h>
#include<thread>
#include<unistd.h>

using namespace std;

#define T_MAX 50
#define SLEEP_DUR 10000
#define NUM_INSERT 1000

list_opt opt;

int clockcycles[T_MAX];

void testList(int i)
{
	clock_t s,e;


	s = clock();
	for(int j = 1; j <= NUM_INSERT; j++)
	{
		opt.add(i * NUM_INSERT + j);
		//usleep(rand() % ((i + 2) * NUM_INSERT));
		//usleep((rand() % 5) * 100);
	}

#if 1
	

	for(int j = NUM_INSERT; j >= 1; j--)
	{
		opt.remove(i * NUM_INSERT + j);
		//usleep(rand() % ((i + 2) * NUM_INSERT));
		//usleep((rand() % 5) * 100);
	}
#endif

	e = clock();

	clockcycles[i] = (e - s);
}

int main()
{
	unsigned int totalTime = 0;

	thread* pThreads[T_MAX];

	for(int i = 0; i < T_MAX; i++)
	{
		pThreads[i] = new thread(testList,i);
	}

	for(int i = 0; i < T_MAX; i++)
	{
		pThreads[i]->join();
		delete(pThreads[i]);
	}

	opt.printAll();

	//printf("\ne = %d, s = %d, Time taken = %f",e,s,((float)(e - s)) / CLOCKS_PER_SEC);
	

	for(int i = 0; i < T_MAX; i++)
	{
		totalTime += clockcycles[i];	
	}

	printf("\nTotal time = %f",((float)totalTime)/CLOCKS_PER_SEC);
	

	return 0;
}
