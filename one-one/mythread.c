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

#define MAX_THREAD 200
#define STACK_SIZE 8196

int thread_count = 0;
static thread_spinlock_t lock;

void thread_init()
{
    thread_spin_init(&lock);
}
/*  
    The futex() system call provides a method
    for waiting until a certain condition be‐
    comes true
*/
static inline int futex(int *uaddr, int futex_op, int val) {
    return syscall(SYS_futex, uaddr, futex_op, val, NULL, NULL, 0);
}

// wrapper function required for clone system call
static int thread_wrapper(void *thread) {
    struct thread *th = (struct thread *)thread;
    // to keep the registers values in the CPU in some place in the memory
    if(sigsetjmp(th->env, 0) == 0)
        th->retval = th->start_routine(th->arg);   
    return 0;
}

/*
DESCRIPTION
       The pthread_create() function starts a new thread in the calling process.  The new 
       thread starts execution by invoking start_routine(); arg is passed as the sole argument of start_routine().
RETURN VALUE
       On success, pthread_create() returns 0; on error, it returns an error number, and the contents of  *thread
       are undefined.
ERRORS
       EAGAIN Insufficient resources to create another thread
       EAGAIN A system-imposed limit on the number of threads was encountered.  
       EINVAL Invalid settings in attr.
       EPERM  No permission to set the scheduling policy and parameters specified in attr.
*/
int thread_create(thread_t *tid, const thread_attr_t *attr, void * (*start_routine)(void *), void *arg)
{   
    thread_spin_lock(&lock);
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
    //mallocing the stack
    //later stack can be created with sbrk function or mmap
    th->stack = (char *)malloc(STACK_SIZE);

    //if stack creation is failed
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
    th->tid = clone(thread_wrapper,
                   th->stack + th->stack_size,
                   CLONE_VM | CLONE_FS | CLONE_FILES |
                   CLONE_SIGHAND |CLONE_THREAD | CLONE_SYSVSEM |
                   CLONE_SETTLS |CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID,
                   th, &th->futex, th, &th->futex);

    if(th->tid == -1)
    {
        free(th->stack);
        free(th);
        thread_spin_unlock(&lock);
        return errno;
    }
    *tid = th->tid;
    thread_count++;
    thread_spin_unlock(&lock);
    enqueue(th);
    return 0;  
}

/*
DESCRIPTION
       The pthread_join() function waits for the thread specified by thread to terminate.  If that thread has al‐ready terminated, then pthread_join() returns immediately.  The thread specified by thread must be joinable.
RETURN VALUE
       On success, pthread_join() returns 0; on error, it returns an error number.
ERRORS
       EDEADLK A  deadlock was detected
       EINVAL thread is not a joinable thread.
       EINVAL Another thread is already waiting to join with this thread.
       ESRCH  No thread with the ID thread could be found
*/
int thread_join(thread_t tid, void **retval)
{
    struct thread *target = search_on_tid(tid);
    if(target == NULL){
        return ESRCH;
    }
    int err = futex(&target->futex, FUTEX_WAIT, target->tid);
    if(err == -1)
        return err;
        
    if(retval)
        *retval = target->retval;
    return 0;
}

/*
DESCRIPTION
       The  pthread_kill()  function  sends the signal sig to thread, a thread in the same process as the caller. The signal is asynchronously directed to thread.
       If sig is 0, then no signal is sent, but error checking is still performed.
RETURN VALUE
       On success, pthread_kill() returns 0; on error, it returns an error number, and no signal is sent.
ERRORS
       EINVAL An invalid signal was specified.
*/
int mthread_kill(thread_t tid, int sig) {
    if(sig == 0)
        return 0;
    // get the id of the calling process or parent process 
    thread_t tgid = getpid();
    int err = tgkill(tgid, tid, sig);
    if(err == -1)
        return errno;
        
    return 0;
}

/*
DESCRIPTION
       The  pthread_exit()  function  terminates  the  calling thread and returns a value via retval that (if the thread is joinable) is available to another thread in the same process that calls pthread_join(3).
RETURN VALUE
       This function does not return to the caller.
ERRORS
       This function always succeeds.
*/
void thread_exit(void *retval)
{
    struct thread *th;
    thread_t tid, pid;
    tid = gettid();
    pid = getpid();

    if(tid == pid)
        return;   
    th = search_on_tid(tid);
    th->retval = retval;
    if(th)
    {
        longjmp(th->env, 1);
    }
    return;
}

/*
DESCRIPTION
       The pthread_equal() function compares two thread identifiers.
RETURN VALUE
       If the two thread IDs are equal, pthread_equal() returns a nonzero value; otherwise, it returns 0.
ERRORS
       This function always succeeds.
*/
int thread_equal(thread_t tid1, thread_t tid2)
{
    if(tid1 == tid2)
        return 1;
    return 0;
}


