#include "mythread.h"
#include <time.h>

void *routine1()
{
    for(int i=0; i < 4; i++)
    {
        printf("%d\n", i);
        sleep(1);
    }
}

int main()
{
    thread_t tid[4];
    //thread_init();
    thread_create(&tid[0], NULL, routine1, NULL);
    thread_join(tid[0], NULL);
    // thread_create(&tid[1], routine1, NULL);
    // thread_join(tid[1], NULL);
    // thread_create(&tid[2], routine3, NULL);
    // thread_create(&tid[3], routine4, NULL);
    printf("\ncontrol back to main\n");
}