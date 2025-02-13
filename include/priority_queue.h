#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NORMAL_PRIORITY 0
#define HIGH_PRIORITY 1

typedef struct Vehicle
{
    int id;
    int lane;
    int priority;
    struct Vehicle *next;
} Vehicle;

typedef struct PriorityQueue
{
    Vehicle *front;
    int size;
} PriorityQueue;

PriorityQueue *createPriorityQueue();
bool isEmpty(PriorityQueue *pq);
void enqueue(PriorityQueue *pq, int id, int lane, int priority);
Vehicle *dequeue(PriorityQueue *pq);
void displayQueue(PriorityQueue *pq);
void freeQueue(PriorityQueue *pq);

#endif
