#include "../include/priority_queue.h"

PriorityQueue *createPriorityQueue()
{
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->front = NULL;
    pq->size = 0;
    return pq;
}

bool isEmpty(PriorityQueue *pq)
{
    return (pq->front == NULL);
}

void enqueue(PriorityQueue *pq, int id, int lane, int priority)
{
    Vehicle *newVehicle = (Vehicle *)malloc(sizeof(Vehicle));
    newVehicle->id = id;
    newVehicle->lane = lane;
    newVehicle->priority = priority;
    newVehicle->next = NULL;

    if (isEmpty(pq) || priority == NORMAL_PRIORITY)
    {
        if (pq->front == NULL)
        {
            pq->front = newVehicle;
        }
        else
        {
            Vehicle *temp = pq->front;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newVehicle;
        }
    }
    else
    {
        newVehicle->next = pq->front;
        pq->front = newVehicle;
    }

    pq->size++;
}

Vehicle *dequeue(PriorityQueue *pq)
{
    if (isEmpty(pq))
    {
        return NULL;
    }

    Vehicle *temp = pq->front;
    pq->front = pq->front->next;
    pq->size--;

    return temp;
}

void displayQueue(PriorityQueue *pq)
{
    Vehicle *temp = pq->front;
    while (temp != NULL)
    {
        printf("Vehicle ID: %d, Lane: %d, Priority: %d\n", temp->id, temp->lane, temp->priority);
        temp = temp->next;
    }
}

void freeQueue(PriorityQueue *pq)
{
    while (!isEmpty(pq))
    {
        Vehicle *temp = dequeue(pq);
        free(temp);
    }
    free(pq);
}
