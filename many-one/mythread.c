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
#include "interrupt.h"
#define MAX_THREAD 200
#define STACK_SIZE 8196

int thread_count = 0;

static thread_spinlock_t lock;
static struct thread *current_running_thread;
static thread_timer_t timer; 

static inline int futex(int *uaddr, int futex_op, int val) {
    return syscall(SYS_futex, uaddr, futex_op, val, NULL, NULL, 0);
}


int thread_create(thread_t *tid, const thread_attr_t *attr, void * (*start_routine)(void *), void *arg)
{   
    disable_interrupt(&timer) ;
    if(tid == NULL){
        return EINVAL;
    }
    //count of the thread exceeds MAX_COUNT
    if(thread_count == MAX_THREAD){
        enable_iterrupt(&timer);
        return EAGAIN;
    }

    //invalid arguments
    if(start_routine == NULL){
        enable_iterrupt(&timer);
        return EINVAL;
    }

    struct thread *thread = (struct thread *)malloc(sizeof(struct thread));
    
    if(thread == NULL)
    {
        enable_iterrupt(&timer);
        return EAGAIN; 
    }

    thread->tid = thread_count++;
    thread->arg = arg;
    thread->start_routine = start_routine;
    thread->retval = NULL;

    enqueue(thread);
    enable_iterrupt(&timer);
    return 0;  
}

int thread_join(thread_t tid, void **retval)
{
    struct thread *target = search_on_tid(tid);
    disable_interrupt(&timer);
    if(target == NULL){
        disable_interrupt(&timer);
        return ESRCH;
    }

    if(current_running_thread->tid == tid)
    {
        disable_interrupt(&timer);
        return EDEADLK;
    }

    int err = futex(&target->futex, FUTEX_WAIT, target->tid);
    if(err == -1)
        return err;
    if(retval)
        *retval = target->retval;
    return 0;
}

int mthread_kill(thread_t tid, int sig) {
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


