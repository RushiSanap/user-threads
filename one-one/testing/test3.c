#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../mythread.h"

void *routine()
{
    printf("Hello world");
}

int main()
{
    thread_t tid;
    int ret;
    thread_init();
    ret = thread_create(&tid, NULL, NULL, NULL);
    if(ret == EINVAL)
        printf("TEST3 PASSED\n");
    else
        printf("TEST3 FAILED\n");
    
}