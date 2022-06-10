#include <stdio.h>
#include "../mythread.h"
#include "../spin_lock.h"
int counter = 0;
int limit = 1000000;
thread_spinlock_t lock;

void *count_to_big(void *arg)
{
    for (int i = 0; i < limit; i++)
    {
        thread_spin_lock(&lock);
        counter++;
        thread_spin_unlock(&lock);
    }
}

int main()
{
    thread_spinlock_t lock;
    thread_spin_init(&lock);
    thread_t c1, c2, c3;
    thread_init();
    int pid1 = thread_create(&c1, NULL, count_to_big, NULL);
    int pid2 = thread_create(&c2, NULL, count_to_big, NULL);
    int pid3 = thread_create(&c3, NULL, count_to_big, NULL);
    int expected = 3*limit;
    printf("Expected counter value: %d\n", 3 * limit);
    thread_join(c1, NULL);
    thread_join(c2, NULL);
    thread_join(c3, NULL);
    printf("Counter value after execution: %d\n", counter);
    int actual = counter;
    if(expected == counter)
    {
        printf("TES7 PASSED\n");
    }
    else
    {
        printf("TES7 FAILED\n");
    }
    return 0;
}