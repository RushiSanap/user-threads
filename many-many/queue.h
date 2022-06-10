
#ifndef _Q_H_
#define _Q_H_
#include "mythread.h"

struct Node
{
    struct thread *th;
    struct Node *next;
};
struct Node *front;
struct Node *rear;

void enqueue(struct thread *th);

struct thread* dequeue();

void display();

struct thread *search_on_tid(thread_t tid);

#endif