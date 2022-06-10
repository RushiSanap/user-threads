#include "mythread.h"
#include <time.h>

void *routine1()
{
    printf("hello\n");
}

int main()
{
    thread_t tid;
    thread_create(&tid, NULL, routine1, NULL);
    thread_join(tid, NULL);
    printf("\ncontrol back to main\n");
}