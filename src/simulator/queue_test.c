#include <stdio.h>
#include "../include/queue.h"

int main()
{
    Queue *q = createQueue("AL1");

    Vehicle v1 = {1, "AL1", 100};
    Vehicle v2 = {2, "AL1", 102};
    Vehicle v3 = {3, "AL1", 104};

    // Test enqueue
    printf("Enqueuing vehicles...\n");
    enqueue(q, v1);
    enqueue(q, v2);
    enqueue(q, v3);
    printf("Queue size: %d\n", getQueueSize(q));

    // Test dequeue
    printf("\nDequeuing vehicles...\n");
    while (!isQueueEmpty(q))
    {
        Vehicle v = dequeue(q);
        printf("Dequeued vehicle: ID=%d, Lane=%s, Time=%d\n",
               v.id, v.lane, v.arrival_time);
    }

    // Clean up
    deleteQueue(q);
    printf("\nQueue deleted successfully\n");

    return 0;
}