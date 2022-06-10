#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include "../mythread.h"
int result_val;

void *increment_one(void *args){
    int *num = (int *)args;
    result_val = *num+1;
    thread_exit(&result_val);
}

int main() {
    thread_t c1;
    int i=1;
    thread_init();
    printf("Expected Value is: %d\n", i+1);
    int mythrd_id = thread_create(&c1,NULL, increment_one, &i);
    sleep(4);
    printf("From main return value is: %d\n", result_val);
    if(result_val==i+1)
        printf("TEST8 PASSED\n");
    else
        printf("TEST8 FAILED\n");
    return 0;
}