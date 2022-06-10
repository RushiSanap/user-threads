#include <stdatomic.h>
#include <assert.h>
#include <errno.h>
#include "spin_lock.h"

static inline int atomic_cas(int *lock_addr, int expected, int desired) {
    return atomic_compare_exchange_strong(lock_addr, &expected, desired);
}

int thread_spin_init(thread_spinlock_t *lock) {
    lock->lock_value = UNLOCKED;
    return 0;
}

int thread_spin_lock(thread_spinlock_t *lock) {
    while (!atomic_cas(&lock->lock_value, UNLOCKED, LOCKED));
    return 0;
}


int thread_spin_trylock(thread_spinlock_t *lock) {
    if(atomic_cas(&lock->lock_value, UNLOCKED, LOCKED))
        return 0;

    return EBUSY;
}

int thread_spin_unlock(thread_spinlock_t *lock) {
    atomic_cas(&lock->lock_value, LOCKED, UNLOCKED);
    return 0;
}