#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <linux/futex.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <asm/prctl.h>
#include <sys/prctl.h>
#include "mythread.h"
#include "spin_lock.h"
#include "queue.h"
#define MAX_THREAD 20
#define STACK_SIZE 8196

int thread_count = 0;

static thread_spinlock_t lock;

int thread_create(thread_t *tid, const thread_attr_t *attr, void * (*start_routine)(void *), void *arg)
{   
    thread_spin_init(&lock);
    //count of the thread exceeds MAX_COUNT
    if(thread_count == MAX_THREAD){
        thread_spin_unlock(&lock);
        return EAGAIN;
    }
        

    //invalid setting in the arguments
    if(start_routine == NULL){
        thread_spin_unlock(&lock);
        return EINVAL;
    }

    struct thread *th = (struct thread *)malloc(sizeof(struct thread));
    
    if(th == NULL)
    {
        thread_spin_unlock(&lock);
        return EAGAIN; 
    }

    if(!th->stack)
    {
        free(th);
        thread_spin_unlock(&lock);
        return ENOMEM;
    }

    th->stack_size = STACK_SIZE;
    th->arg = arg;
    th->start_routine = start_routine;
    th->retval = NULL;

    thread_count++;
    thread_spin_unlock(&lock);
    enqueue(th);
    return 0;  
}

int thread_join(thread_t tid, void **retval)
{
    struct thread *target = search_on_tid(tid);
    if(target == NULL){
        return ESRCH;
    }
        
    if(retval)
        *retval = target->retval;
    return 0;
}

int mthread_kill(thread_t tid, int sig)
{    
    return 0;
}

void thread_exit(void *retval)
{
    return;
}

int thread_equal(thread_t tid1, thread_t tid2)
{
    if(tid1 == tid2)
        return 1;
    return 0;
}

