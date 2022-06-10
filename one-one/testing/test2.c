#include <stdio.h>
#include <unistd.h>
#include "../mythread.h"

void *routine()
{
    for(int i=0; i < 4; i++)
    {
        printf("%d\n", i);
        sleep(1);
    }
}

int main()
{
    thread_t tid;
    int ret;
    thread_create(&tid, NULL, routine, NULL);
    thread_init();
    ret = thread_join(tid, NULL);
    if(ret == 0)
        printf("TEST2 PASSED\n");
    else
        printf("TEST2 FAILED\n");
}