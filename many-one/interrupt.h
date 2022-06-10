#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <sys/time.h>

#define TIMER   (suseconds_t) 10000

typedef struct itimerval thread_timer_t;

void enable_iterrupt(thread_timer_t *timer);

void disable_interrupt(thread_timer_t *timer);

#endif