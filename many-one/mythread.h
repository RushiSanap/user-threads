#ifndef _MYTHREAD_H_
#define _MYTHREAD_H_

#include <stdio.h>
#include <setjmp.h>
#include <stdint.h>
#include <sys/types.h>

typedef unsigned long int thread_t;

enum {
    DETACHED,
    JOINABLE,
};

typedef enum thread_state {
    RUNNING,
    READY,
    FINISHED,
    WAITING
}thread_state_t;

struct thread_attr
{
};

typedef struct thread_attr thread_attr_t;
int thread_attr_init(thread_attr_t *attr);
int thread_attr_destroy(thread_attr_t *attr);

//thread structure
struct thread {
    //thread id
    thread_t tid;

    //base pointer of the stack
    void *stack;

    //size of the stack
    int stack_size;

    //futex
    int32_t futex;

    //function which is given to the thread
    void *(*start_routine)(void *);

    //argument to be passed to the function
    void *arg;

    //return value of the function
    void *retval;

    //kernel thread id
    thread_t kernel_tid;

    // For exiting safely
    sigjmp_buf env;

    // TID to be joined to once finished
    thread_t joined_on;

    // For signal handling
    sigset_t pending_signal;

    // Detachment type
    int joinable;
};

int thread_create(thread_t *tid, const thread_attr_t *attr, void * (*start_routine)(void *), void *arg);

int thread_join(thread_t tid, void **retval);

int thread_kill(thread_t tid, int sig);

void thread_exit(void *retval);

int thread_equal(thread_t tid1, thread_t tid2);

struct thread* search_thread_id(thread_t tid);

#endif

