#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../mythread.h"

int g = 0;
void *myThreadFun(void *vargp)
{
	int *myid = (int *)vargp;
	static int s = 0;
	++s; ++g;
	printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g);
}

int main()
{
	int i;
	thread_t tid;
	thread_init();
	for (i = 0; i < 3; i++)
		thread_create(&tid, NULL, myThreadFun, (void *)&tid);
	for (i = 0; i < 3; i++)
		thread_join(tid, NULL);
	if(g == 6)
	{
		printf("TEST6 PASSED\n");
	}
	else
	{
		printf("TEST6 FAILED\n");
	}
	return 0;
}
