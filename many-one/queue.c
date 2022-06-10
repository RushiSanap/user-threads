#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int isEmpty()
{
    return front == NULL;
}

void enqueue(struct thread *th)
{
    struct Node *t;
    t=(struct Node*)malloc(sizeof(struct Node));
    if(t == NULL)
        return;
    else
    {
        t->th=th;
        t->next=NULL;
    }
    if(front==NULL)
        front=rear=t;
    else
    {
        rear->next=t;
        rear=t;
    }
}

struct thread* dequeue()
{
    struct thread *th = NULL;
    struct Node* t;

    if(front==NULL)
        printf("Queue is Empty\n");
    else
    {
        th=front->th;
        t=front;
        front=front->next;
        free(t);
    }
    return th;
}

void display()
{
    struct Node *p=front;
    while(p)
    {
        printf("%ld ",p->th->tid);
        p=p->next;
    }
    printf("\n");
}

struct thread * search_on_tid(thread_t tid)
{
    struct Node *p=front;
    while(p)
    {
        if(p->th->tid == tid)
            return p->th;
        p=p->next;
    }
    return NULL;
}