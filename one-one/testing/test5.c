#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../mythread.h"

void *routine()
{
    for(int i = 0; i < 3; i++)
    {
        printf("hello from rushikesh\n");
        sleep(1);
    }
}

int main()
{
    thread_t tid1, tid2;
    int ret;
    thread_init();
    thread_create(&tid1, NULL, routine, NULL);
    ret = thread_join(tid1, NULL);
    if(ret == 0)
        printf("TEST5 PASSED\n");
    else
        printf("TEST5 FAILED\n");
    return 0;
}