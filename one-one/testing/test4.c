#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../mythread.h"

void *routine()
{
    printf("Hello world");
}

int main()
{
    thread_t tid1, tid2;
    int ret;
    thread_init();
    thread_create(&tid1, NULL, routine, NULL);
    ret = thread_join(tid2, NULL);
    if(ret == ESRCH)
        printf("TEST4 PASSED\n");
    else
        printf("TEST4 FAILED\n");
    
}