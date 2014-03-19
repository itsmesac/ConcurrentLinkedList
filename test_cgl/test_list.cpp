#include<iostream>
#include<thread>
#include "list_cgl.cpp"
#include<time.h>
#include<thread>
#include<unistd.h>

using namespace std;

#define T_MAX 40
#define SLEEP_DUR 10000
#define NUM_INSERT 1000

list_cgl cgl;

int clockcycles_a[T_MAX];
int clockcycles_r[T_MAX];
int clockcycles_c[T_MAX];
int clockcycles_arc[T_MAX];

void testList(int i)
{
	clock_t s_a,e_a;
	clock_t s_r,e_r;
	clock_t s_c,e_c;
	clock_t s_arc,e_arc;

	s_arc = clock();

	s_a = s_arc;
	for(int j = 1; j <= NUM_INSERT; j++)
	{
		cgl.add(i * NUM_INSERT + j);
		//usleep(rand() % ((i + 2) * NUM_INSERT));
		//usleep((rand() % 5) * 100);
	}
	e_a = clock();

	s_c = clock();
	for(int j = 1; j <= NUM_INSERT; j++)
	{
		cgl.contains(i * NUM_INSERT + j);
	}
	e_c = clock();

	s_r = clock();
	for(int j = NUM_INSERT; j >= 1; j--)
	{
		cgl.remove(i * NUM_INSERT + j);
		//usleep(rand() % ((i + 2) * NUM_INSERT));
		//usleep((rand() % 5) * 100);
	}
	e_r = clock();

	e_arc = e_r;

	clockcycles_a[i] = (e_a - s_a);
	clockcycles_r[i] = (e_r - s_r);
	clockcycles_c[i] = (e_c - s_c);
	clockcycles_arc[i] = (e_arc - s_arc);
}

int main()
{
	unsigned int totalTime_a = 0;
	unsigned int totalTime_r = 0;
	unsigned int totalTime_c = 0;
	unsigned int totalTime_arc = 0;

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

	cgl.printAll();

	//printf("\ne = %d, s = %d, Time taken = %f",e,s,((float)(e - s)) / CLOCKS_PER_SEC);
	

	for(int i = 0; i < T_MAX; i++)
	{
		totalTime_a += clockcycles_a[i];	
		totalTime_r += clockcycles_r[i];	
		totalTime_c += clockcycles_c[i];	
		totalTime_arc += clockcycles_arc[i];	
	}

	printf("\nTotal time for add = %f",((float)totalTime_a)/CLOCKS_PER_SEC);
	printf("\nTotal time for remove = %f",((float)totalTime_r)/CLOCKS_PER_SEC);
	printf("\nTotal time for contains = %f",((float)totalTime_c)/CLOCKS_PER_SEC);
	printf("\nTotal time for concurrent add/remove/contains = %f",((float)totalTime_arc)/CLOCKS_PER_SEC);
	

	return 0;
}
