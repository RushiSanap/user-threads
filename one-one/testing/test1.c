#include <stdio.h>
#include "../mythread.h"

void *routine()
{
    printf("Hello world\n");
}

int main()
{
    thread_t tid;
    int ret;
    thread_init();
    ret = thread_create(&tid, NULL, routine, NULL);
    if(ret == 0)
        printf("TEST1 PASSED\n");
    else
        printf("TEST1 FAILED\n");
}