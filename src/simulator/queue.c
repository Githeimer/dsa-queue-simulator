// queue.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../queue.h"

Queue *createQueue(const char *lane)
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL)
    {
        return NULL;
    }

    q->front = q->rear = NULL;
    q->size = 0;
    strncpy(q->lane, lane, 2);
    q->lane[2] = '\0';

    return q;
}

bool isQueueEmpty(Queue *q)
{
    return (q->front == NULL);
}

void enqueue(Queue *q, Vehicle vehicle)
{
    QNode *newNode = (QNode *)malloc(sizeof(QNode));
    if (newNode == NULL)
    {
        return;
    }

    newNode->vehicle = vehicle;
    newNode->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }

    q->size++;
}

Vehicle dequeue(Queue *q)
{
    Vehicle emptyVehicle = {0, "", 0};

    if (isQueueEmpty(q))
    {
        return emptyVehicle;
    }

    QNode *temp = q->front;
    Vehicle vehicle = temp->vehicle;

    q->front = q->front->next;
    q->size--;

    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    free(temp);
    return vehicle;
}

void deleteQueue(Queue *q)
{
    while (!isQueueEmpty(q))
    {
        dequeue(q);
    }
    free(q);
}

int getQueueSize(Queue *q)
{
    return q->size;
}