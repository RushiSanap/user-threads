#ifndef _SPIN_LOCK_H_
#define _SPIN_LOCK_H_

#include "mythread.h"

enum{
    UNLOCKED,
    LOCKED
};

// spinlock structure
struct thread_spinlock {
    // value of spinlock
    int lock_value;
};

typedef struct thread_spinlock thread_spinlock_t;

int thread_spin_init(thread_spinlock_t *lock_value);

int thread_spin_lock(thread_spinlock_t *lock_value);

int thread_spin_trylock(thread_spinlock_t *lock_value);

int thread_spin_unlock(thread_spinlock_t *lock_value);

#endif